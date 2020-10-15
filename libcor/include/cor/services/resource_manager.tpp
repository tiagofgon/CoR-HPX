#ifdef COR_RESOURCE_MANAGER_HPP

#include <typeinfo> 

#include "cor/elements/dynamic_organizer.hpp"
#include "cor/elements/static_organizer.hpp"


namespace cor {

// CreateLocal que retorna o cliente object do recurso criado
template <typename T, typename ... Args>
std::unique_ptr<T> ResourceManager::CreateLocal(idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args)
{

    // std::cout << "idp_t_createLocal_resource_manager: "<< ctx << std::endl;
    // std::cout << "idp_t: " << ctx << std::endl;

    auto idp = GenerateIdp();
    // std::cout << "idp2: " << idp << std::endl;

    // (std::cout << ... << args); std::cout << std::endl;
    std::unique_ptr<T> rsc = std::make_unique<T>(idp, std::forward<Args>(args)...);
    // std::cout << "aqui5" << std::endl;
    rsc = AllocateResource(idp, ctx, name, std::move(rsc), ctrl);
    // std::cout << "aqui6" << std::endl;
    // insert association between gids and idps
    InsertIdp(idp, rsc->GetGid()); // Informar o componente global da associação idp-gid
    // std::cout << "aqui7" << std::endl;
    // _predecessors.emplace(idp, ctx);
    InsertPredecessorIdp(idp, ctx);
    
    // std::cout << "aqui8" << std::endl;
    return GetLocalResource<T>(idp);
}


// CreateLocal que retorna o cliente object do recurso criado
template <typename T, typename ... Args>
idp_t ResourceManager::CreateRemote(idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args)
{
    auto idp = GenerateIdp();

    {
        // lock to access resource manager variables
        std::lock_guard<std::mutex> lk(_mtx);

        // Se o ctx não estiver registado no componente global retorna erro
        if ( FindIdp(ctx) == false ) {
            throw std::runtime_error("Resource " + std::to_string(ctx) + " does not exist in global component!(CreateRemote)");
        } else {
            // Ir buscar o gid do ctx remoto
            auto ctx_gid = GetGidFromIdp(ctx);
            auto ctx_locality = AttachResourceRemote(ctx_gid, idp, name);

            // Criar o recurso na localidade do remote Domain
            std::unique_ptr<T> rsc_remote = std::make_unique<T>(idp, ctx_locality, std::forward<Args>(args)...);
        }

    }

    return idp;
}



template <typename T, typename ... Args>
idp_t ResourceManager::Create(idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args)
{
    // std::cout << "idp_t_createLocal_resource_manager: "<< ctx << std::endl;
    // std::cout << "idp_t: " << ctx << std::endl;

    auto idp = GenerateIdp();
    // std::cout << "idp2: " << idp << std::endl;

    // (std::cout << ... << args); std::cout << std::endl;
    std::unique_ptr<T> rsc = std::make_unique<T>(idp, std::forward<Args>(args)...);

    rsc = AllocateResource(idp, ctx, name, std::move(rsc), ctrl);
    
    // insert association between gids and idps
    InsertIdp(idp, rsc->GetGid()); // Informar o componente global da associação idp-gid

    // _predecessors.emplace(idp, ctx);
    InsertPredecessorIdp(idp, ctx);

    

    return idp;
}



template <typename T>
std::unique_ptr<T> ResourceManager::AllocateResource(idp_t idp, idp_t ctx, std::string const& name, std::unique_ptr<T> rsc, std::string const& ctrl)
{
    idp_t ori_ctx;
    {
        // lock to access resource manager variables
        std::lock_guard<std::mutex> lk(_mtx);

        // Ir buscar o idp original do ctx, pode ser local ou nao
        
        auto alias_it =_alias.find(ctx);
        if ( alias_it == _alias.end() ) { // Se o ctx nao pertencer ao _alias
            ori_ctx = ctx;
        } else { 
            ori_ctx = alias_it->second;
        }
    }

    // std::cout << "aqui 12" << std::endl;

    // O ori_ctx tem de existir no componente global, para ir buscar o seu gid
    if ( FindIdp(ori_ctx) == false ) {
        throw std::runtime_error("Resource " + std::to_string(ori_ctx) + " does not exist!(AllocateResource)");
    } else { // Cria um objeto componente cliente que está associado ao gid que corresponde ao ctx
        // std::cout << "aqui 13" << std::endl;
        auto gid = GetGidFromIdp(ori_ctx);
// std::cout << "aqui 14" << std::endl;
        // attach resource to the context
        AttachResource(ctx, gid, idp, name);
    }


// std::cout << "aqui 15" << std::endl;

    {
        // lock to access resource manager variables
        std::lock_guard<std::mutex> lk(_mtx);
        std::vector<std::string> res = rsc->GetComponentHierarchy();
        bool flag=false;
        for(auto const& str : res) {
            if(str == "DynamicOrganizer") { // Se tiver o elemento organiador DynamicOrganizer
                // dynamicOrganizer_idps.insert(idp);
                InsertDynamicOrganizer_idps(idp);
                std::cout << "adicionado ao dynamicOrganizer_idps " << idp << std::endl;
                break;
            } 
            if(str == "StaticOrganizer") { // Se tiver o elemento organiador StaticOrganizer
                staticOrganizer_idps.insert(idp);
                InsertStaticOrganizer_idps(idp);
                std::cout << "adicionado ao StaticOrganizer " << idp << std::endl;
                break;
            }
        }
    }

    return rsc;
}

template <typename T>
std::unique_ptr<T> ResourceManager::CreateReference(idp_t idp, idp_t ctx, std::string const& name, std::string const& ctrl)
{

    auto alias = GenerateIdp();
    std::unique_ptr<T> rsc;

    // Se o idp não estiver registado no componente global retorna erro
    if ( FindIdp(idp) == false ) {
        throw std::runtime_error("Resource " + std::to_string(idp) + " does not exist!(CreateReference)");
    } else { // Cria um objeto componente cliente que está associado ao gid
        auto gid = GetGidFromIdp(idp);
        // create local client resource, but the server component resource is remote or not
        std::unique_ptr<T> rsc1 = std::make_unique<T>(alias, std::move(gid));
        // attach resource to the context
        rsc = AllocateResource(alias, ctx, name, std::move(rsc1), ctrl);
    }

    {
        // lock to access resource manager variables
        std::lock_guard<std::mutex> lk(_mtx);

        // insert relationship of ancestry and alias
        // _predecessors.emplace(alias, ctx);
        
        _alias.emplace(alias, idp);
    }
    InsertPredecessorIdp(alias, ctx);
    return GetLocalResource<T>(alias);
    
}

template <typename T, typename ... Args>
std::unique_ptr<T> ResourceManager::CreateCollective(idm_t rank, idp_t comm, idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args)
{

    std::unique_ptr<T> rsc_ptr;

    auto comm_ori_idp = ResolveIdp(comm); // vai buscar o idp original da clausura identificado por comm
    std::string basename = name + "a";

    if (rank == 0) {
        std::cout << "dentro do rank 0 em resource_manager - CreateCollective" << std::endl;
        rsc_ptr = CreateLocal<T>(ctx, name, ctrl, std::forward<Args>(args)...);
        auto future = hpx::register_with_basename(basename, rsc_ptr->GetGid(), 0);
        // SendStaticGroupCCIdp(comm_ori_idp, rsc_ptr->Idp());
    } else {
        // auto idp = GetStaticGroupCCIdp(comm_ori_idp);

		auto gid = hpx::find_from_basename(basename, 0).get();
        
        typedef Resource::Idp_action_Resource action_type;
        auto idp = hpx::async<action_type>(gid).get();

        rsc_ptr = CreateReference<T>(idp, ctx, name, ctrl);
    }

    // SynchronizeStaticGroup(comm_ori_idp);

    return rsc_ptr;
}


// template <typename T>
// ResourcePtr<T> ResourceManager::GetLocalResource(idp_t idp)
// {
//     // auto cst_obj = GetConsistencyObject(idp);
//     // return ResourcePtr<T>{idp, cst_obj};

//     ResourcePtr<T> rsc_ptr; // estou a inventar para nao apagar esta funcao
//     return rsc_ptr;
// }

template <typename T>
std::unique_ptr<T> ResourceManager::GetLocalResource(idp_t idp)
{
    
    auto alias_it = _alias.find(idp);
    auto ori_idp = (alias_it == _alias.end()) ? idp : alias_it->second;

    if ( FindIdp(ori_idp) == false ) {
        throw std::runtime_error("Resource " + std::to_string(ori_idp) + " does not exist!(GetLocalResource)");
    } else { // Cria um objeto componente cliente que está associado ao gid
        auto gid = GetGidFromIdp(ori_idp);
        std::unique_ptr<T> new_object = std::make_unique<T>(idp, std::move(gid));
        return new_object;
    }

}

}

#endif
