#include "cor/services/resource_manager.hpp"

#include "cor/services/controller.hpp"
#include "cor/resources/group.hpp"
#include "cor/system/macros.hpp"

#include <typeinfo>
#include <cassert>
#include <stdexcept>
#include <algorithm>

#include "cor/resources/domain_client.hpp"
#include "cor/resources/closure_client.hpp"

// to remove
#include <iostream>
#include <cassert>
#include <thread>

namespace cor {

ResourceManager::ResourceManager(Controller *ctrl, bool first) :
    _ctrl{ctrl},
    _is_main_mgr{first},
    _predecessors{},
    _mtx{},
    _mtx2{},
    dynamicOrganizer_idps{},
    staticOrganizer_idps{}
{
    InsertDynamicOrganizer_idps(cor::MetaDomain); // insert the meta-domain idp into the dynamicOrganizer set 
}

ResourceManager::~ResourceManager() = default;

void ResourceManager::CreateInitialContext(std::string const& ctrl)
{
    if (_is_main_mgr) {
        CreateMetaDomain(ctrl);
    }
    else {
        FindMetaDomain(ctrl);
    }
}

void ResourceManager::CleanInitialContext()
{
    // //std::cout << "BEGIN CleanInitialContext" << std::endl;
    // GetConsistencyObject(cor::MetaDomain)->DecrementLocalReferenceCounter();
    // //std::cout << "END CleanInitialContext" << std::endl;
}

void ResourceManager::CreateMetaDomain(std::string const& ctrl)
{
    // create meta-domain resource
    std::unique_ptr<Domain_Client> rsc = std::make_unique<Domain_Client>(cor::MetaDomain, "");

    // insert association between idp and gid on gids map
    InsertIdp(cor::MetaDomain, rsc->GetGid()); // Inform the global component of the idp-gid association, as well as the local association
    
    // insert relationship of ancestry
    InsertPredecessorIdp(cor::MetaDomain, cor::MetaDomain);

    auto future = hpx::register_with_basename("MetaDomain", rsc->GetGid());

    // self join of meta-domain resource
    rsc->Join(cor::MetaDomain, "MetaDomain");

    // std::cout << "-- Criado o Meta-Dominio -- " << cor::MetaDomain << std::endl;
}

void ResourceManager::FindMetaDomain(std::string const& ctrl)
{   
    // find meta-domain resource
    auto meta_domain_gid = hpx::find_from_basename("MetaDomain", 0).get();

    // insert association between idp and gid on gids map
    InsertIdp(cor::MetaDomain, meta_domain_gid); // Inform the global component of the idp-gid association, as well as the local association

    // insert relationship of ancestry
    InsertPredecessorIdp(cor::MetaDomain, cor::MetaDomain);

    // std::cout << "-- Encontrado o Meta-Dominio -- " << cor::MetaDomain << std::endl;
}

bool ResourceManager::ContainsResource(idp_t idp)
{
    _mtx2.lock();

    bool res;
    auto it = _predecessors.find(idp);
    if ( it == _predecessors.end() ) {
       res = false;
    } else { 
        res = true;
    }

    _mtx2.unlock();

    return res;
}

std::string ResourceManager::SearchResource(idp_t idp)
{
    // std::string ctrl;
    // auto total_pods = _ctrl->GetTotalPods();

    // {
    //     std::unique_lock<std::mutex> lk(_mtx);

    //     if (_sr_vars.find(idp) == _sr_vars.end()) {
    //         _sr_vars.emplace(idp, std::make_tuple(0, total_pods, ""));
    //         _ctrl->SendSearchResourceRequest(idp);
    //     }

    //     while (std::get<0>(_sr_vars[idp]) != std::get<1>(_sr_vars[idp]))
    //         _sr_cv[idp].wait(lk);

    //     ctrl = std::get<2>(_sr_vars[idp]);
    // }

    // if (ctrl == "")
    //     throw std::runtime_error("Resource " + std::to_string(idp) + " does not exists!");

    // return ctrl;
    return "asdas";
}


unsigned int ResourceManager::GetNumDomains()
{
    auto gid = GetGidFromIdp(cor::MetaDomain);
    std::unique_ptr<Domain_Client> meta_domain = std::make_unique<Domain_Client>(std::move(gid));
    return meta_domain->GetTotalMembers();
}

std::vector<idp_t> ResourceManager::GetDomains()
{
    auto gid = GetGidFromIdp(cor::MetaDomain);
    std::unique_ptr<Domain_Client> meta_domain = std::make_unique<Domain_Client>(std::move(gid));
    return meta_domain->GetMemberList();
}



idp_t ResourceManager::GetDomainIdp(idp_t idp)
{
    _mtx2.lock();

    auto ret = idp;
    auto ctx = _predecessors.at(ret);

    while (ctx != cor::MetaDomain) {
        ret = ctx;
        ctx = _predecessors.at(ret);
    }

    _mtx2.unlock();

    return ret;
}

void ResourceManager::InsertPredecessorIdp(idp_t idp, idp_t idp_predecessor)
{
    _mtx2.lock();

    _predecessors.emplace(idp, idp_predecessor);
    _ctrl->InsertPredecessorIdpGlobal(idp, idp_predecessor);

    _mtx2.unlock();
}

bool ResourceManager::FindPredecessorIdp(idp_t idp)
{
    _mtx2.lock();
    bool res;
    auto it = _predecessors.find(idp);
    if ( it == _predecessors.end() ) {
        res = _ctrl->FindPredecessorIdpGlobal(idp);
    } else { 
        res = true;
    }

    _mtx2.unlock();

    return res;
}

idp_t ResourceManager::GetPredecessorIdp(idp_t idp)
{
    _mtx2.lock();

    idp_t res;
    auto it = _predecessors.find(idp);
    if (it != _predecessors.end()) {
        res = it->second;
    } else { // in case the idp is remote
        res = _ctrl->GetPredecessorIdpGlobal(idp);
    }

    _mtx2.unlock();

    return res;
}

void ResourceManager::RemovePredecessorIdp(idp_t idp)
{
    _predecessors.erase(idp);
    return _ctrl->RemovePredecessorIdpGlobal(idp);
}


idp_t ResourceManager::ResolveIdp(idp_t idp)
{
    // lock to access resource manager variables
    _mtx2.lock();

    idp_t res;
    auto it = _alias.find(idp);
    if (it == _alias.end())
        res = idp;
    else
        res = it->second;

    _mtx2.unlock();

    return res;
}

void ResourceManager::EraseResource(idp_t idp)
{
    // std::unique_lock<std::mutex> lk(_mtx);

    // auto rsc_cst_obj = _cst_objs.at(idp);
    // _cst_objs.erase(idp);
    // delete rsc_cst_obj;

    // _sync_free[idp].notify_all();
}

void ResourceManager::DeallocateResource(idp_t idp)
{
    _mtx2.lock();

    std::cout << "BEGIN <" << _ctrl->GetName() << "> DEALLOCATE RESOURCE " << std::to_string(idp) << std::endl;

    if (idp == cor::MetaDomain)
        RemovePredecessorIdp(idp);

    // if (dynamic_cast<DynamicOrganizer*>(rsc) != nullptr /* || dynamic_cast<StaticOrganizer*>(rsc) != nullptr */) {
    if(FindDynamicOrganizer_idps(idp) == true) {
        // get all keys with same value from predecessors
        std::vector<idp_t> rscs;
        std::for_each(_predecessors.begin(), _predecessors.end(),
            [&] (const auto& pair) -> void {
                if (pair.second == idp)
                    rscs.push_back(pair.first);
            });

        for (const auto& rsc_idp: rscs) {

            std::cout << "--->>> " << std::to_string(rsc_idp) << std::endl;
            RemovePredecessorIdp(rsc_idp);
        }
    }

    if (idp != cor::MetaDomain) {
        auto ctx = GetPredecessorIdp(idp);
        auto ctx_gid = GetGidFromIdp(ctx);
        
        if(FindDynamicOrganizer_idps(idp) == true) {
            std::unique_ptr<Domain_Client> rsc = std::make_unique<Domain_Client>(std::move(ctx_gid));
            rsc->Leave(idp);
        } else {
            if(FindStaticOrganizer_idps(idp) == true) {
                std::unique_ptr<Closure_Client> rsc = std::make_unique<Closure_Client>(std::move(ctx_gid));
                rsc->Leave(idp);
            }
        }
    }

    _mtx2.unlock();
    std::cout << "END <" << _ctrl->GetName() << "> DEALLOCATE RESOURCE " << std::to_string(idp) << std::endl;
}

idp_t ResourceManager::GenerateIdp()
{
    return _ctrl->GenerateIdp();
}

void ResourceManager::InsertIdp(idp_t idp, hpx::id_type gid)
{
    _mtx2.lock();

    gids.emplace(idp, gid);
    _ctrl->InsertIdpGlobal(idp, gid);

    _mtx2.unlock();
}

bool ResourceManager::FindIdp(idp_t idp)
{
    //std::cout << "-- " << idp << std::endl;
    _mtx2.lock();
    //std::cout << "2FindIdp(ctx)" << idp << std::endl;
    bool res;
    auto it = _predecessors.find(idp);
    if ( it == _predecessors.end() ) {
       res = _ctrl->FindIdpGlobal(idp);
    } else { 
        res = true;
    }

    _mtx2.unlock();
    
    return res;
}

hpx::id_type ResourceManager::GetGidFromIdp(idp_t idp)
{
    _mtx2.lock();

    hpx::id_type res;
    auto it = gids.find(idp);
    if (it != gids.end()) {
        res = it->second;
    } else { // in case the idp is remote
        res = _ctrl->GetGidFromIdpGlobal(idp);
    }

    _mtx2.unlock();

    return res;
}

void ResourceManager::RemoveIdp(idp_t idp)
{
    _mtx2.lock();

    gids.erase(idp);
    _ctrl->RemoveIdpGlobal(idp);

    _mtx2.unlock();
}

void ResourceManager::InsertDynamicOrganizer_idps(idp_t idp)
{
    _mtx2.lock();

    dynamicOrganizer_idps.insert(idp);
    _ctrl->InsertDynamicOrganizer_idpsGlobal(idp);

    _mtx2.unlock();
}

void ResourceManager::InsertStaticOrganizer_idps(idp_t idp)
{
    _mtx2.lock();

    staticOrganizer_idps.insert(idp);
    _ctrl->InsertStaticOrganizer_idpsGlobal(idp);

    _mtx2.unlock();
}

bool ResourceManager::FindDynamicOrganizer_idps(idp_t idp)
{
    _mtx2.lock();

    bool res;
    if(dynamicOrganizer_idps.find(idp) != dynamicOrganizer_idps.end()) {
        res = true;
    }
    else {
        res = _ctrl->FindDynamicOrganizer_idpsGlobal(idp);
    }
    
    _mtx2.unlock();

    return res;
}

bool ResourceManager::FindStaticOrganizer_idps(idp_t idp)
{
    _mtx2.lock();

    bool res;
    if(staticOrganizer_idps.find(idp) != staticOrganizer_idps.end()) {
        res = true;
    }
    else {
        res = _ctrl->FindStaticOrganizer_idpsGlobal(idp);
    }

    _mtx2.unlock();

    return res;
}

void ResourceManager::InsertAgentMailbox(idp_t idp, hpx::id_type gid)
{
    _mtx2.lock();

    _agents_mailbox.emplace(idp, gid);
    _ctrl->InsertAgentMailboxGlobal(idp, gid);

    _mtx2.unlock();
}

hpx::id_type ResourceManager::GetAgentMailbox(idp_t idp)
{
    _mtx2.lock();

    hpx::id_type res;   
    auto it = _agents_mailbox.find(idp);
    if (it != _agents_mailbox.end()) {
        res = it->second;
    } else { // no caso do idp ser remoto
        res = _ctrl->GetAgentMailboxGlobal(idp);
    }

    _mtx2.unlock();

    return res;
}

void ResourceManager::AttachResource(idp_t ctx, hpx::id_type ctx_gid, idp_t idp, std::string const& name)
{
    // Add to ctx resource organizer element
    if(FindDynamicOrganizer_idps(ctx) == true) {
        std::unique_ptr<Domain_Client> rsc = std::make_unique<Domain_Client>(std::move(ctx_gid));
        rsc->Join(idp, name);
    } else if(FindStaticOrganizer_idps(ctx) == true) {
        //std::cout << "ResourceManager::AttachResource 1" << std::endl;
        std::unique_ptr<Closure_Client> rsc = std::make_unique<Closure_Client>(std::move(ctx_gid));
        //std::cout << "ResourceManager::AttachResource 2" << std::endl;
        auto IdpGlobal = rsc->IdpGlobal();
        //std::cout << "IdpGlobal: " << IdpGlobal << std::endl;
        rsc->Join(idp, name);
        //std::cout << "ResourceManager::AttachResource 3" << std::endl;
    } else {
        throw std::runtime_error("Resource " + std::to_string(ctx) + " does not have an organizer!(AttachResource)");
    }

}

hpx::id_type ResourceManager::AttachResourceRemote(hpx::id_type ctx_gid, idp_t idp, std::string const& name)
{
    // Criar um componente cliente localmente, que se refere ao componente remoto
    std::unique_ptr<Domain_Client> rsc_remote = std::make_unique<Domain_Client>(std::move(ctx_gid));

    // Retornar a localidade de onde o Domain está
    auto locality = rsc_remote->GetLocalityGID();
    // attach resource to the context (remote domain)
    rsc_remote->Join(idp, name);

    return locality;
}



}
