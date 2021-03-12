#ifdef COR_AGENT_HPP


namespace cor {
    
template <typename R, typename ... P>
Agent<R(P...)>::~Agent() = default;

template <typename R, typename ... P>
Agent<R(P...)>::Agent(idp_t idp, std::function<R(P...)> const& f) :
    Resource{idp},
    _executor{idp, f},
    _mailBox{idp}
{
    // std::cout << "Criado um componente \"Agent\", com idp: " << idp << std::endl;
}

template <typename R, typename ... P>
Agent<R(P...)>::Agent(idp_t idp, std::string const& module, std::string const& function) :
    Resource{idp},
    _executor{idp, module, function},
    _mailBox{idp}
{
    // std::cout << "Criado um componente \"Agent\", com idp: " << idp << std::endl;
}




/* Executor's interface */
template <typename R, typename ... P>
template <typename ... Args>
hpx::future<R> Agent<R(P...)>::Run1(Args... args)
{
    return _executor.Run(args... );
}

template <typename R, typename ... P>
template <typename ... Args>
hpx::future<R> Agent<R(P...)>::Run2(Args&&... args)
{
    return _executor.Run(std::forward<Args>(args)...);
}

template <typename R, typename ... P>
void Agent<R(P...)>::ChangeIdp(idp_t idp)
{
    return _executor.ChangeIdp(idp);
}

template <typename R, typename ... P>
void Agent<R(P...)>::ResumeIdp()
{
    return _executor.ResumeIdp();
}

template <typename R, typename ... P>
idp_t Agent<R(P...)>::CurrentIdp()
{
    return _executor.CurrentIdp();
}

template <typename R, typename ... P>
idp_t Agent<R(P...)>::OriginalIdp()
{
    return _executor.OriginalIdp();
}

template <typename R, typename ... P>
idp_t Agent<R(P...)>::GetExecutorIdp()
{
    return _executor.GetExecutorIdp();
}




/* Mailbox's interface */
template <typename R, typename ... P>
void Agent<R(P...)>::Send1(idp_t dest, Message const& msg)
{
    return _mailBox.Send(dest, msg);
}

template <typename R, typename ... P>
void Agent<R(P...)>::Send2(std::vector<idp_t> const& dests, Message const& msg)
{
    return _mailBox.Send(dests, msg);
}

template <typename R, typename ... P>
Message Agent<R(P...)>::Receive1()
{
    return _mailBox.Receive();
}

template <typename R, typename ... P>
Message Agent<R(P...)>::Receive2(idp_t source)
{
    return _mailBox.Receive(source);
}

template <typename R, typename ... P>
void Agent<R(P...)>::Broadcast(idp_t clos, Message const& msg)
{
    return _mailBox.Broadcast(clos, msg);
}

template <typename R, typename ... P>
void Agent<R(P...)>::Send3(idm_t rank, idp_t clos, Message const& msg)
{
    return _mailBox.Send(rank, clos, msg);
}

template <typename R, typename ... P>
Message Agent<R(P...)>::Receive3(idm_t rank, idp_t clos)
{
    return _mailBox.Receive(rank, clos);
}

template <typename R, typename ... P>
hpx::id_type Agent<R(P...)>::GetMailboxGid()
{
    return _mailBox.GetGid();
}




}


#endif
