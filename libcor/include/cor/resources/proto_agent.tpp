#ifdef COR_PROTO_AGENT_HPP


namespace cor {

template <typename R, typename ... P>
ProtoAgent<R(P...)>::~ProtoAgent() = default;

template <typename R, typename ... P>
ProtoAgent<R(P...)>::ProtoAgent(idp_t idp, unsigned int pod_id, std::function<R(P...)> const& f) :
    Resource{idp},
    _executor{idp, pod_id, f}
{
    //std::cout << "Criado um componente \"ProtoAgent\", com idp: " << idp << std::endl;
}

template <typename R, typename ... P>
ProtoAgent<R(P...)>::ProtoAgent(idp_t idp, unsigned int pod_id, std::string const& module, std::string const& function) :
    Resource{idp},
    _executor{idp, pod_id, module, function}
{
    //std::cout << "Criado um componente \"ProtoAgent\", com idp: " << idp << std::endl;
}


/* Executor's interface */
template <typename R, typename ... P>
template <typename ... Args>
hpx::future<R> ProtoAgent<R(P...)>::Run1(Args... args)
{
    return _executor.Run(args...);
}

template <typename R, typename ... P>
template <typename ... Args>
hpx::future<R> ProtoAgent<R(P...)>::Run2(Args&&... args)
{
    return _executor.Run(std::forward<Args>(args)...);
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
