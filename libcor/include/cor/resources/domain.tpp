#ifdef COR_DOMAIN_HPP


namespace cor {

template <typename T>
std::unique_ptr<T> Domain::GetLocalResource(idp_t idp)
{
    return _container.GetLocalResource<T>(idp);
}

template <typename T>
std::unique_ptr<T> Domain::CreateLocal_agent(idp_t ctx, std::string const& name, hpx::function<void(void*)> const& func)
{
    return _container.CreateLocal_agent<T>(ctx, name, func);
    //return nullptr;
}

template <typename T, typename ... Args>
std::unique_ptr<T> Domain::CreateLocal(idp_t ctx, std::string const& name, Args&& ... args)
{
    return _container.CreateLocal<T, Args...>(ctx, name, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
idp_t Domain::Create(idp_t ctx, std::string const& name, Args&& ... args)
{
    return _container.Create<T, Args...>(ctx, name, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
idp_t Domain::CreateRemote(idp_t ctx, std::string const& name, Args&& ... args)
{
    return _container.CreateRemote<T, Args...>(ctx, name, std::forward<Args>(args)...);
}

template <typename T>
std::unique_ptr<T> Domain::CreateReference(idp_t idp, idp_t ctx, std::string const& name)
{
    return _container.CreateReference<T>(idp, ctx, name);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Domain::CreateCollective1(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args)
{
    return _container.CreateCollective<T, Args...>(ctx, name, total_members, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Domain::CreateCollective2(idp_t active_rsc_idp, idp_t clos, idp_t ctx, std::string const& name, Args&& ... args)
{
    return _container.CreateCollective<T, Args...>(active_rsc_idp, clos, ctx, name, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
void Domain::Run(idp_t idp, Args&&... args)
{
    return _container.Run<T, Args...>(idp, std::forward<Args>(args)...);
}

template <typename T>
void Domain::Wait(idp_t idp)
{
    return _container.Wait<T>(idp);
}

template <typename T>
auto Domain::Get(idp_t idp)
{
    return _container.Get<T>(idp);
}


}


#endif
