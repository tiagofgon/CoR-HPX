#ifdef COR_PROTO_AGENT_HPP

namespace cor {

template <typename R, typename ... P>
ProtoAgent<R(P...)>::ProtoAgent() = default;

template <typename R, typename ... P>
ProtoAgent<R(P...)>::ProtoAgent(idp_t idp, std::function<R(P...)> const& f) :
    base_type(idp),
    executor{idp, f}
{
    std::cout << "Criado um componente \"ProtoAgent\", com idp: " << idp << std::endl;
}

template <typename R, typename ... P>
ProtoAgent<R(P...)>::ProtoAgent(idp_t idp, std::string const& module, std::string const& function) :
    base_type(idp),
    executor{idp, module, function}
{
    std::cout << "Criado um componente \"ProtoAgent\", com idp: " << idp << std::endl;
}

template <typename R, typename ... P>
ProtoAgent<R(P...)>::~ProtoAgent() = default;

// template <typename R, typename ... P>
// ProtoAgent<R(P...)>::ProtoAgent(ProtoAgent<R(P...)>&&) noexcept = default;

// template <typename R, typename ... P>
// ProtoAgent<R(P...)>& ProtoAgent<R(P...)>::operator=(ProtoAgent<R(P...)>&&) noexcept = default;



/* Executor interface */
template <typename R, typename ... P>
template <typename ... Args>
void ProtoAgent<R(P...)>::Run(Args&&... args)
{
    return executor.Run(std::forward<Args>(args)... );
}

template <typename R, typename ... P>
void ProtoAgent<R(P...)>::Wait()
{
    return executor.Wait();
}

template <typename R, typename ... P>
R ProtoAgent<R(P...)>::Get()
{
    return executor.Get();
}

template <typename R, typename ... P>
void ProtoAgent<R(P...)>::ChangeIdp(idp_t idp)
{
    return executor.ChangeIdp(idp);
}

template <typename R, typename ... P>
void ProtoAgent<R(P...)>::ResumeIdp()
{
    return executor.ResumeIdp();
}

template <typename R, typename ... P>
idp_t ProtoAgent<R(P...)>::CurrentIdp()
{
    return executor.CurrentIdp();
}

template <typename R, typename ... P>
idp_t ProtoAgent<R(P...)>::OriginalIdp()
{
    return executor.OriginalIdp();
}

template <typename R, typename ... P>
idp_t ProtoAgent<R(P...)>::GetExecutorIdp()
{
    return executor.GetExecutorIdp();
}



}

#endif
