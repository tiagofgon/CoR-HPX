#ifdef COR_CONTAINER_HPP


#include "cor/system/system.hpp"
#include "cor/utils/utils.hpp"

#include "cor/system/pod_client.hpp"


namespace cor {

template <typename T>
std::unique_ptr<T> Container::GetLocalResource(idp_t idp)
{
    return global::pod->GetLocalResource<T>(idp);
}

// template <typename T>
// std::unique_ptr<T> Container::CreateLocal_agent(idp_t ctx, std::string const& name, hpx::function<void(int)> const& func)
// {
//     return global::pod->CreateLocal_agent<T>(ctx, name, func);
// }

template <typename T, typename ... Args>
std::unique_ptr<T> Container::CreateLocal(idp_t ctx, std::string const& name, Args ... args)
{
    // std::cout << "Container::CreateLocal" << std::endl;
    return global::pod->CreateLocal<T, Args...>(ctx, name, args...);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Container::CreateLocal_test(idp_t ctx, std::string const& name, Args ... args)
{
    std::cout << "Container::CreateLocal_test" << std::endl;
    return global::pod->CreateLocal_test<T, Args...>(ctx, name, args...);
    //return nullptr;
}

template <typename T, typename ... Args>
idp_t Container::Create(idp_t ctx, std::string const& name, Args ... args)
{
    // If the ctx resource belongs to the _predecessors list, the resource is created in this pod, if it does not create remotely
    if (global::pod->ContainsResource(ctx)) {
        //std::cout << "Create - local" << std::endl;
        return global::pod->Create<T, Args...>(ctx, name, args...);
    }
    else {
        //std::cout << "Create - remoto" << std::endl;
        return CreateRemote<T, Args...>(ctx, name, args...);
    }
}

// ACABAR AQUI
template <typename T, typename ... Args>
idp_t Container::CreateRemote(idp_t ctx, std::string const& name, Args ... args)
{
    auto ctrl = global::pod->SearchResource(ctx);
    ctrl[1] = 'R'; // nao serve para nada isto, está aqui porque veio assim da versao anterior
    return global::pod->CreateRemote<T, Args...>(ctx, name, ctrl, args...);
}

// template <typename T, typename ... Args>
// idp_t Container::Create(idp_t ctx, std::string const& name, Args ... args)
// {   
//     // If the ctx resource belongs to the _predecessors list, the resource is created in this pod, if it does not create remotely
//     if (global::pod->ContainsResource(ctx)) {
//         std::cout << "Create - local" << std::endl;
//         return global::pod->Create<T, Args...>(ctx, name, args...);
//     }
//     else {
//         std::cout << "Create - remoto" << std::endl;
//         return CreateRemote<T, Args...>(ctx, name, args...);
//     }
// }

template <typename T>
std::unique_ptr<T> Container::CreateReference(idp_t idp, idp_t ctx, std::string const& name)
{
    return global::pod->CreateReference<T>(idp, ctx, name);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Container::CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args ... args)
{
    return global::pod->CreateCollective<T, Args...>(ctx, name, total_members, args...);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Container::CreateCollective(idp_t active_rsc_idp, idp_t clos, idp_t ctx, std::string const& name, Args ... args)
{
    return global::pod->CreateCollective<T, Args...>(active_rsc_idp, clos, ctx, name, args...);
}

// ACABAR AQUI
template <typename T, typename ... Args>
auto Container::Run(idp_t idp, Args... args)
{
    // auto ctrl = global::pod->SearchResource(idp);
    // ctrl[1] = 'R';
    // return global::rpc->Run<T>(idp, ctrl, args...);
    auto agent = GetLocalResource<T>(idp);
    return agent->Run(hpx::launch::async, args...).get();
    //return agent->Run(args...);
}


}


#endif
