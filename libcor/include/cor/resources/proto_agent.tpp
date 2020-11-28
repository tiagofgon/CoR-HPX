#ifdef COR_PROTO_AGENT_HPP


namespace cor {

template <typename R, typename ... P>
ProtoAgent<R(P...)>::~ProtoAgent() = default;

template <typename R, typename ... P>
ProtoAgent<R(P...)>::ProtoAgent(idp_t idp, std::function<R(P...)> const& f) :
    Resource{idp},
    _executor{idp, f}
{
    std::cout << "Criado um componente \"ProtoAgent\", com idp: " << idp << std::endl;
}

template <typename R, typename ... P>
ProtoAgent<R(P...)>::ProtoAgent(idp_t idp, std::string const& module, std::string const& function) :
    Resource{idp},
    _executor{idp, module, function}
{
    std::cout << "Criado um componente \"ProtoAgent\", com idp: " << idp << std::endl;
}


/* Executor's interface */
template <typename R, typename ... P>
void ProtoAgent<R(P...)>::Run_void(std::shared_ptr<void> arg)
{
    return _executor.Run_void(arg);
}

template <typename R, typename ... P>
template <typename ... Args>
void ProtoAgent<R(P...)>::Run(Args&&... args)
{
    return _executor.Run(std::forward<Args>(args)... );
}

template <typename R, typename ... P>
void ProtoAgent<R(P...)>::Wait()
{
    return _executor.Wait();
}

template <typename R, typename ... P>
R ProtoAgent<R(P...)>::Get()
{
    return _executor.Get();
}

template <typename R, typename ... P>
void ProtoAgent<R(P...)>::ChangeIdp(idp_t idp)
{
    return _executor.ChangeIdp(idp);
}

template <typename R, typename ... P>
void ProtoAgent<R(P...)>::ResumeIdp()
{
    return _executor.ResumeIdp();
}

template <typename R, typename ... P>
idp_t ProtoAgent<R(P...)>::CurrentIdp()
{
    return _executor.CurrentIdp();
}

template <typename R, typename ... P>
idp_t ProtoAgent<R(P...)>::OriginalIdp()
{
    return _executor.OriginalIdp();
}

template <typename R, typename ... P>
idp_t ProtoAgent<R(P...)>::GetExecutorIdp()
{
    return _executor.GetExecutorIdp();
}


}


#endif
