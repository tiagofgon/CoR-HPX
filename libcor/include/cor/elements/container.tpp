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

template <typename T>
std::unique_ptr<T> Container::CreateLocal_agent(idp_t ctx, std::string const& name, hpx::function<void(void*)> const& func)
{
    return global::pod->CreateLocal_agent<T>(ctx, name, func);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Container::CreateLocal(idp_t ctx, std::string const& name, Args&& ... args)
{
    return global::pod->CreateLocal<T, Args...>(ctx, name, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
idp_t Container::Create(idp_t ctx, std::string const& name, Args&& ... args)
{
    // If the ctx resource belongs to the _predecessors list, the resource is created in this pod, if it does not create remotely
    if (global::pod->ContainsResource(ctx)) {
        // std::cout << "Create - local" << std::endl;
        return global::pod->Create<T, Args...>(ctx, name, std::forward<Args>(args)...);
    }
    else {
        // std::cout << "Create - remoto" << std::endl;
        return CreateRemote<T, Args...>(ctx, name, std::forward<Args>(args)...);
    }
}

// ACABAR AQUI
template <typename T, typename ... Args>
idp_t Container::CreateRemote(idp_t ctx, std::string const& name, Args&& ... args)
{
    auto ctrl = global::pod->SearchResource(ctx);
    ctrl[1] = 'R';
    return global::pod->CreateRemote<T, Args...>(ctx, name, ctrl, std::forward<Args>(args)...);
}

// template <typename T, typename ... Args>
// idp_t Container::Create(idp_t ctx, std::string const& name, Args&& ... args)
// {   
//     // If the ctx resource belongs to the _predecessors list, the resource is created in this pod, if it does not create remotely
//     if (global::pod->ContainsResource(ctx)) {
//         std::cout << "Create - local" << std::endl;
//         return global::pod->Create<T, Args...>(ctx, name, std::forward<Args>(args)...);
//     }
//     else {
//         std::cout << "Create - remoto" << std::endl;
//         return CreateRemote<T, Args...>(ctx, name, std::forward<Args>(args)...);
//     }
// }

template <typename T>
std::unique_ptr<T> Container::CreateReference(idp_t idp, idp_t ctx, std::string const& name)
{
    return global::pod->CreateReference<T>(idp, ctx, name);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Container::CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args)
{
    return global::pod->CreateCollective<T, Args...>(ctx, name, total_members, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Container::CreateCollective(idp_t active_rsc_idp, idp_t clos, idp_t ctx, std::string const& name, Args&& ... args)
{
    return global::pod->CreateCollective<T, Args...>(active_rsc_idp, clos, ctx, name, std::forward<Args>(args)...);
}

// ACABAR AQUI
template <typename T, typename ... Args>
void Container::Run(idp_t idp, Args&&... args)
{
    // auto ctrl = global::pod->SearchResource(idp);
    // ctrl[1] = 'R';
    // return global::rpc->Run<T>(idp, ctrl, std::forward<Args>(args)...);
    auto agent = GetLocalResource<T>(idp);
    agent->Run(std::forward<Args>(args)...);
}

template <typename T>
void Container::Wait(idp_t idp)
{
    // auto ctrl = global::pod->SearchResource(idp);
    // ctrl[1] = 'R';
    // global::rpc->Wait<T>(idp, ctrl);
    auto agent = GetLocalResource<T>(idp);
    agent->Wait();
}

template <typename T>
auto Container::Get(idp_t idp)
{
    // auto ctrl = global::pod->SearchResource(idp);
    // ctrl[1] = 'R';
    // return global::rpc->Get<T>(idp, ctrl);
    auto agent = GetLocalResource<T>(idp);
    return agent->Get();
}


}


#endif
