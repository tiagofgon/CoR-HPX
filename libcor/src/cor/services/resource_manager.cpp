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
    dynamicOrganizer_idps{},
    staticOrganizer_idps{}
{
    // std::cout << "cacacac1" << std::endl;
    InsertDynamicOrganizer_idps(cor::MetaDomain); // inserir o idp do meta-domain no conjunto dos recursos com dynamicOrganizer
    // std::cout << "cacacac2" << std::endl;
    // std::cout << "ResourceManager contrutor2" << std::endl;
}

ResourceManager::~ResourceManager() = default;

void ResourceManager::CreateInitialContext(std::string const& ctrl)
{
    if (_is_main_mgr) {
        // std::cout << "--------------AQUI------------" << std::endl;
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
    // std::cout << "aqui11" << std::endl;
    // insert association between idp and gid on gids map
    InsertIdp(cor::MetaDomain, rsc->GetGid()); // Informar o componente global da associação idp-gid, assim como na associação local
    
    // std::cout << "aqui1" << std::endl;
    // insert relationship of ancestry
    InsertPredecessorIdp(cor::MetaDomain, cor::MetaDomain);
    // std::cout << "aqui2" << std::endl;
    auto future = hpx::register_with_basename("MetaDomain", rsc->GetGid());
    // std::cout << "aqui3" << std::endl;
    // self join of meta-domain resource
    rsc->Join(cor::MetaDomain, "MetaDomain");
    // std::cout << "aqui4" << std::endl;
    // std::cout << "-- Criado o Meta-Dominio -- " << cor::MetaDomain << std::endl;
}

void ResourceManager::FindMetaDomain(std::string const& ctrl)
{   
    // std::this_thread::sleep_for (std::chrono::seconds(5));

    // find meta-domain resource
    auto meta_domain_gid = hpx::find_from_basename("MetaDomain", 0).get();


    // insert association between idp and gid on gids map
    InsertIdp(cor::MetaDomain, meta_domain_gid); // Informar o componente global da associação idp-gid, assim como na associação local


    // insert relationship of ancestry
    InsertPredecessorIdp(cor::MetaDomain, cor::MetaDomain);

    // std::cout << "-- Encontrado o Meta-Dominio -- " << cor::MetaDomain << std::endl;
}

bool ResourceManager::ContainsResource(idp_t idp)
{
    std::unique_lock<std::mutex> lk(_mtx);
    return FindPredecessorIdp(idp);
}

// Resource *ResourceManager::GetResource(idp_t idp)
// {
//     // auto cobj = GetConsistencyObject(idp);
//     // return cobj->GetResource();
//     return nullptr;
// }

// hpx::id_type ResourceManager::GetResource(idp_t idp)
// {
//     std::lock_guard<std::mutex> lk(_mtx);

//     auto alias_it = _alias.find(idp);
//     auto ori_idp = (alias_it == _alias.end()) ? idp : alias_it->second;

//     if ( gids.find(ori_idp) == gids.end() ) {
//         std::cout << "ERRO: Recurso com idp " << idp <<" não existe - CreateLocal3" << std::endl;
//     } else { // Cria um objeto componente cliente que está associado ao gid que corresponde ao ori_idp
//         auto gid = gids.find(ori_idp)->second;

//     // return gid of resource idp
//     return gid;

// }

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


// ACABAR ISTO
unsigned int ResourceManager::GetTotalDomains()
{
    // auto rsc = GetResource(cor::MetaDomain);
    // auto meta_domain = dynamic_cast<Domain*>(rsc);
    // auto total_members = meta_domain->GetTotalMembers();

    auto total_members = 9;
    return total_members - 1;
}

idp_t ResourceManager::GetDomainIdp(idp_t idp)
{
    std::unique_lock<std::mutex> lk(_mtx); // shared_lock

    auto ret = idp;
    auto ctx = _predecessors.at(ret);

    while (ctx != cor::MetaDomain) {
        ret = ctx;
        ctx = _predecessors.at(ret);
    }

    return ret;
}

void ResourceManager::InsertPredecessorIdp(idp_t idp, idp_t idp_predecessor)
{
    std::unique_lock<std::mutex> lk(_mtx); // shared_lock

    _predecessors.emplace(idp, idp_predecessor);
    return _ctrl->InsertPredecessorIdpGlobal(idp, idp_predecessor);
}

bool ResourceManager::FindPredecessorIdp(idp_t idp)
{
    std::unique_lock<std::mutex> lk(_mtx); // shared_lock

    auto it = _predecessors.find(idp);
    if ( it == _predecessors.end() ) {
       return _ctrl->FindPredecessorIdpGlobal(idp);
    } else { 
        return true;
    }

}

idp_t ResourceManager::GetPredecessorIdp(idp_t idp)
{
    // lock to access resource manager variables
    std::unique_lock<std::mutex> lk(_mtx); // shared_lock

    auto it = _predecessors.find(idp);
    if (it != _predecessors.end()) {
        return it->second;
    } else { // no caso do idp ser remoto
        return _ctrl->GetPredecessorIdpGlobal(idp);
    }
}

void ResourceManager::RemovePredecessorIdp(idp_t idp)
{
    _predecessors.erase(idp);
    return _ctrl->RemovePredecessorIdpGlobal(idp);
}


idp_t ResourceManager::ResolveIdp(idp_t idp)
{
    // lock to access resource manager variables
    std::unique_lock<std::mutex> lk(_mtx); //shared_lock

    auto it = _alias.find(idp);
    if (it == _alias.end())
        return idp;
    else
        return it->second;
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
    std::unique_lock<std::mutex> lk(_mtx);

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

    std::cout << "END <" << _ctrl->GetName() << "> DEALLOCATE RESOURCE " << std::to_string(idp) << std::endl;
}








idp_t ResourceManager::GenerateIdp()
{
    return _ctrl->GenerateIdp();
}

void ResourceManager::InsertIdp(idp_t idp, hpx::id_type gid)
{
    std::unique_lock<std::mutex> lk(_mtx); //shared_lock

    gids.emplace(idp, gid);
    return _ctrl->InsertIdpGlobal(idp, gid);
}

bool ResourceManager::FindIdp(idp_t idp)
{
    std::unique_lock<std::mutex> lk(_mtx); //shared_lock

    auto it = _predecessors.find(idp);
    if ( it == _predecessors.end() ) {
       return _ctrl->FindIdpGlobal(idp);
    } else { 
        return true;
    }
}

hpx::id_type ResourceManager::GetGidFromIdp(idp_t idp)
{
    std::unique_lock<std::mutex> lk(_mtx); //shared_lock

    auto it = gids.find(idp);
    if (it != gids.end()) {
        return it->second;
    } else { // no caso do idp ser remoto
        return _ctrl->GetGidFromIdpGlobal(idp);
    }
}

void ResourceManager::RemoveIdp(idp_t idp)
{
    std::unique_lock<std::mutex> lk(_mtx); //shared_lock

    gids.erase(idp);
    return _ctrl->RemoveIdpGlobal(idp);
}




void ResourceManager::InsertDynamicOrganizer_idps(idp_t idp)
{
    dynamicOrganizer_idps.insert(idp);
    // std::cout << "cacacac3" << std::endl;
    return _ctrl->InsertDynamicOrganizer_idpsGlobal(idp);
}

void ResourceManager::InsertStaticOrganizer_idps(idp_t idp)
{
    staticOrganizer_idps.insert(idp);
    return _ctrl->InsertStaticOrganizer_idpsGlobal(idp);
}

bool ResourceManager::FindDynamicOrganizer_idps(idp_t idp)
{
    if(dynamicOrganizer_idps.find(idp) != dynamicOrganizer_idps.end()) {
        return true;
    }
    return _ctrl->FindDynamicOrganizer_idpsGlobal(idp);
}

bool ResourceManager::FindStaticOrganizer_idps(idp_t idp)
{
    if(staticOrganizer_idps.find(idp) != staticOrganizer_idps.end()) {
        return true;
    }
    return _ctrl->FindStaticOrganizer_idpsGlobal(idp);
}

void ResourceManager::InsertAgentMailbox(idp_t idp, hpx::id_type gid)
{
    _agents_mailbox.emplace(idp, gid);
    return _ctrl->InsertAgentMailboxGlobal(idp, gid);
}

hpx::id_type ResourceManager::GetAgentMailbox(idp_t idp)
{
    std::unique_lock<std::mutex> lk(_mtx); //shared_lock

    auto it = _agents_mailbox.find(idp);
    if (it != _agents_mailbox.end()) {
        return it->second;
    } else { // no caso do idp ser remoto
        return _ctrl->GetAgentMailboxGlobal(idp);
    }
}





/*
void ResourceManager::DummyInsertWorldContext(idp_t idp, std::string const& name, Resource *rsc, std::string const& ctrl)
{
    if (dynamic_cast<Domain*>(rsc) != nullptr) {
        if (idp == cor::MasterDomain) {
            std::cout << "ANTES" << std::endl;
            Create<cor::Group>(cor::ResourceWorld, idp, "ResourceWorld", true, ctrl, "");
            std::cout << "DEPOIS" << std::endl;
        }
        else
            CreateReference<cor::Group>(cor::ResourceWorld, idp, "ResourceWorld", ctrl);
    }

    auto rsc_world = GetLocalResource<cor::Group>(cor::ResourceWorld);
    rsc_world->Join(idp, name);
}
*/

// std::vector<std::string> ResourceManager::GetComponentHierarchy(hpx::id_type gid) 
// {
//     typedef Resource::Idp_action_resource_ action_type;
//     auto asd = hpx::async<action_type>(gid).get();
//     std::cout << "#####-- idp: " << asd << std::endl;

//     auto dcc = new Domain_Client(std::move(gid));
//     return dcc->GetComponentHierarchy();
// }

void ResourceManager::AttachResource(idp_t ctx, hpx::id_type ctx_gid, idp_t idp, std::string const& name)
{
    // Adicionar ao elemento organizador do recurso ctx
    if(FindDynamicOrganizer_idps(ctx) == true) {
        std::unique_ptr<Domain_Client> rsc = std::make_unique<Domain_Client>(std::move(ctx_gid));
        rsc->Join(idp, name);
    } else if(FindStaticOrganizer_idps(ctx) == true) {
        std::unique_ptr<Closure_Client> rsc = std::make_unique<Closure_Client>(std::move(ctx_gid));
        rsc->Join(idp, name);
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
