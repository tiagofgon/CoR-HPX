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

template <typename T, typename ... Args>
std::unique_ptr<T> Container::CreateLocal(idp_t ctx, std::string const& name, Args&& ... args)
{
    return global::pod->CreateLocal<T, Args...>(ctx, name, std::forward<Args>(args)...);
}

// ACABAR AQUI
template <typename T, typename ... Args>
idp_t Container::CreateRemote(idp_t ctx, std::string const& name, Args&& ... args)
{
    auto ctrl = global::pod->SearchResource(ctx);
    ctrl[1] = 'R';
    // return global::rpc->Create<T>(ctx, name, ctrl, std::forward<Args>(args)...);
    return global::pod->CreateRemote<T>(ctx, name, ctrl, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
idp_t Container::Create(idp_t ctx, std::string const& name, Args&& ... args)
{   
    // Se o recurso ctx pertencer à lista _predecessors, o recurso é criado neste pod, se não cria remoto
    if (global::pod->ContainsResource(ctx))
        return global::pod->Create<T>(ctx, name, std::forward<Args>(args)...);
    else
        return CreateRemote<T>(ctx, name, std::forward<Args>(args)...);
}

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
}

template <typename T>
void Container::Wait(idp_t idp)
{
    // auto ctrl = global::pod->SearchResource(idp);
    // ctrl[1] = 'R';
    // global::rpc->Wait<T>(idp, ctrl);
}

template <typename T>
auto Container::Get(idp_t idp)
{
    // auto ctrl = global::pod->SearchResource(idp);
    // ctrl[1] = 'R';
    // return global::rpc->Get<T>(idp, ctrl);
}


}






#endif
