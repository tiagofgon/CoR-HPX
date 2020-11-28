#ifdef COR_RESOURCE_MANAGER_HPP

#include <typeinfo> 

#include "cor/elements/dynamic_organizer.hpp"
#include "cor/elements/static_organizer.hpp"

namespace cor {

class Mailbox;

// CreateLocal que retorna o cliente object do recurso criado
template <typename T>
std::unique_ptr<T> ResourceManager::CreateLocal_agent(idp_t ctx, std::string const& name, std::string const& ctrl, hpx::function<void(void*)> const& func)
{
    auto idp = GenerateIdp();

    // // (std::cout << ... << args); std::cout << std::endl;
    std::unique_ptr<T> rsc = std::make_unique<T>(idp, func);
    rsc = AllocateResource(idp, ctx, name, std::move(rsc), ctrl);
    // InsertIdp(idp, rsc->GetGid()); // insert association between gids and idps
    // InsertPredecessorIdp(idp, ctx); // insert association between idps and predecessors
    
    return GetLocalResource<T>(idp);
    //return nullptr;
}

// CreateLocal que retorna o cliente object do recurso criado
template <typename T, typename ... Args>
std::unique_ptr<T> ResourceManager::CreateLocal(idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args)
{
    auto idp = GenerateIdp();

    // (std::cout << ... << args); std::cout << std::endl;
    std::unique_ptr<T> rsc = std::make_unique<T>(idp, std::forward<Args>(args)...);
    rsc = AllocateResource(idp, ctx, name, std::move(rsc), ctrl);
    // InsertIdp(idp, rsc->GetGid()); // insert association between gids and idps
    // InsertPredecessorIdp(idp, ctx); // insert association between idps and predecessors
    
    return GetLocalResource<T>(idp);
}

// CreateLocal que retorna o cliente object do recurso criado
template <typename T, typename ... Args>
idp_t ResourceManager::Create(idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args)
{
    // std::cout << "ResourceManager:: 1" << std::endl;
    auto idp = GenerateIdp();
    std::unique_ptr<T> rsc = std::make_unique<T>(idp, std::forward<Args>(args)...);
    // std::cout << "ResourceManager:: 2" << std::endl;
    rsc = AllocateResource(idp, ctx, name, std::move(rsc), ctrl);
    // // std::cout << "ResourceManager:: 3" << std::endl;
    // InsertIdp(idp, rsc->GetGid()); // insert association between gids and idps
    // // std::cout << "ResourceManager:: 4" << std::endl;
    // InsertPredecessorIdp(idp, ctx);
    // std::cout << "ResourceManager:: 5" << std::endl;
    
    return idp;
}

template <typename T, typename ... Args>
idp_t ResourceManager::CreateRemote(idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args)
{
    auto idp = GenerateIdp();
    {
        // lock to access resource manager variables
        // std::lock_guard<std::mutex> lk(_mtx);
        // std::cout << "1FindIdp(ctx)" << ctx << std::endl;
        // Se o ctx não estiver registado no componente global retorna erro
        if ( FindIdp(ctx) == false ) {
            std::cout << "erro" << std::endl;
            throw std::runtime_error("Resource " + std::to_string(ctx) + " does not exist in global component!(CreateRemote)");
        } else {
            // Ir buscar o gid do ctx remoto
            auto ctx_gid = GetGidFromIdp(ctx);
            // retornar a localidade do ctx remoto e adicionar ao seu elemento organizador o idp criado
            auto ctx_locality = AttachResourceRemote(ctx_gid, idp, name);
            // std::cout << "Localidade do dominio remoto: " << ctx_locality << std::endl;
            // Criar o recurso na localidade do remote Domain
            std::unique_ptr<T> rsc_remote = std::make_unique<T>(idp, ctx_locality, std::forward<Args>(args)...);
            // insert association between gids and idps
            InsertIdp(idp, rsc_remote->GetGid()); // Informar o componente global da associação idp-gid

            // _predecessors.emplace(idp, ctx);
            InsertPredecessorIdp(idp, ctx);
        }

    }

    return idp;
}

template <typename T>
std::unique_ptr<T> ResourceManager::AllocateResource(idp_t idp, idp_t ctx, std::string const& name, std::unique_ptr<T> rsc, std::string const& ctrl)
{
    idp_t ori_ctx;
    // std::cout << "ResourceManager::AllocateResource 1" << std::endl;
    // lock to access resource manager variables
    _mtx2.lock();
    // pick up the original ctx idp, it can be local or not
    auto alias_it =_alias.find(ctx);
    if ( alias_it == _alias.end() ) { // If the ctx does not belong to _alias
        ori_ctx = ctx;
    } else { 
        ori_ctx = alias_it->second;
    }
    _mtx2.unlock();
    
    // std::cout << "ResourceManager::AllocateResource 2" << std::endl;
    // ori_ctx must exist in the global component, to fetch their gid
    if (FindIdp(ori_ctx) == false) {
        throw std::runtime_error("Resource " + std::to_string(ori_ctx) + " does not exist!(AllocateResource)");
    } 
    
    
    

    InsertIdp(idp, rsc->GetGid()); // insert association between gids and idps
    InsertPredecessorIdp(idp, ctx); // insert association between idps and predecessors

    
    
    // std::cout << "ResourceManager::AllocateResource 3" << std::endl;

    typedef typename T::organizer element;
    if(typeid(element) == typeid(StaticOrganizer))
    { 
            InsertStaticOrganizer_idps(idp);
            //std::cout << "adicionado ao StaticOrganizer " << idp << std::endl;
    }
    else if(typeid(element) == typeid(DynamicOrganizer))
    {
            InsertDynamicOrganizer_idps(idp);
            //std::cout << "adicionado ao dynamicOrganizer_idps " << idp << std::endl;
    }
    else if(typeid(element) == typeid(Mailbox))
    {
            InsertAgentMailbox(idp, rsc->GetMailboxGid());
            //std::cout << "adicionado ao _agents_mailbox " << idp << std::endl;
    }
    // std::cout << "ResourceManager::AllocateResource 4" << std::endl;

    // Creates a client component object that is associated with the gid that corresponds to the ctx
    auto gid = GetGidFromIdp(ori_ctx);
    AttachResource(ctx, gid, idp, name); // attach resource to the context

    return rsc;
}

template <typename T>
std::unique_ptr<T> ResourceManager::CreateReference(idp_t idp, idp_t ctx, std::string const& name, std::string const& ctrl)
{

    auto alias = GenerateIdp();
    std::unique_ptr<T> rsc;

    // if the idp is not registered in the global component it returns an error
    if ( FindIdp(idp) == false ) {
        throw std::runtime_error("Resource " + std::to_string(idp) + " does not exist!(CreateReference)");
    } else { // Cria um objeto componente cliente que está associado ao gid
        auto gid = GetGidFromIdp(idp);
        // create local client resource, but the server component resource is remote or not
        std::unique_ptr<T> rsc1 = std::make_unique<T>(alias, std::move(gid));
        // attach resource to the context
        rsc = AllocateResource(alias, ctx, name, std::move(rsc1), ctrl);
    }

    
    // lock to access resource manager variables
    _mtx2.lock();

    _alias.emplace(alias, idp);

    _mtx2.unlock();
    

    InsertPredecessorIdp(alias, ctx); // insert relationship of ancestry and alias
    return GetLocalResource<T>(alias);
    
}

template <typename T, typename ... Args>
std::unique_ptr<T> ResourceManager::CreateCollective(idm_t rank, idp_t comm, idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args)
{

    std::unique_ptr<T> rsc_ptr;

    auto comm_ori_idp = ResolveIdp(comm); // get the original closure idp identified by comm
    std::string basename = name + "a";

    if (rank == 0) {
        // std::cout << "dentro do rank 0 em resource_manager - CreateCollective" << std::endl;
        rsc_ptr = CreateLocal<T>(ctx, name, ctrl, std::forward<Args>(args)...);
        auto future = hpx::register_with_basename(basename, rsc_ptr->GetGid(), 0);
    } else {
		auto gid = hpx::find_from_basename(basename, 0).get();
        
        typedef Resource::Idp_action_Resource action_type;
        auto idp = hpx::async<action_type>(gid).get();

        rsc_ptr = CreateReference<T>(idp, ctx, name, ctrl);
    }

    return rsc_ptr;
}

template <typename T>
std::unique_ptr<T> ResourceManager::GetLocalResource(idp_t idp)
{
    auto alias_it = _alias.find(idp);
    auto ori_idp = (alias_it == _alias.end()) ? idp : alias_it->second;

    if ( FindIdp(ori_idp) == false ) {
        throw std::runtime_error("Resource " + std::to_string(ori_idp) + " does not exist!(GetLocalResource)");
    } else { // creates a client component object that is associated with gid
        auto gid = GetGidFromIdp(ori_idp);
        std::unique_ptr<T> new_object = std::make_unique<T>(idp, std::move(gid));
        return new_object;
    }

}


}


#endif
