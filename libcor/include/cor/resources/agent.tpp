#ifdef COR_AGENT_HPP

namespace cor {

template <typename R, typename ... P>
Agent<R(P...)>::Agent() = default;

template <typename R, typename ... P>
Agent<R(P...)>::Agent(idp_t idp, std::function<R(P...)> const& f) :
    ResourceNonMigrable{idp},
    executor{idp, f},
    _mailBox{idp}
    // _mailBox{idp}
{
    // _mailBox_gid = hpx::local_new<Mailbox>(idp).get();
    // _mailBox(Mailbox_Client(idp));
    std::cout << "Criado um componente \"Agent\", com idp: " << idp << std::endl;
}

template <typename R, typename ... P>
Agent<R(P...)>::Agent(idp_t idp, std::string const& module, std::string const& function) :
    ResourceNonMigrable{idp},
    executor{idp, module, function},
    _mailBox{idp}
    // _mailBox{idp}
{
    // _mailBox_gid = hpx::local_new<Mailbox>(idp).get();
    // _mailBox();
    std::cout << "Criado um componente \"Agent\", com idp: " << idp << std::endl;
}

template <typename R, typename ... P>
Agent<R(P...)>::~Agent() = default;

// template <typename R, typename ... P>
// Agent<R(P...)>::Agent(Agent<R(P...)>&&) noexcept = default;

// template <typename R, typename ... P>
// Agent<R(P...)>& Agent<R(P...)>::operator=(Agent<R(P...)>&&) noexcept = default;



/* Executor interface */
template <typename R, typename ... P>
template <typename ... Args>
void Agent<R(P...)>::Run(Args&&... args)
{
    return executor.Run(std::forward<Args>(args)... );
}

template <typename R, typename ... P>
void Agent<R(P...)>::Wait()
{
    return executor.Wait();
}

template <typename R, typename ... P>
R Agent<R(P...)>::Get()
{
    return executor.Get();
}

template <typename R, typename ... P>
void Agent<R(P...)>::ChangeIdp(idp_t idp)
{
    return executor.ChangeIdp(idp);
}

template <typename R, typename ... P>
void Agent<R(P...)>::ResumeIdp()
{
    return executor.ResumeIdp();
}

template <typename R, typename ... P>
idp_t Agent<R(P...)>::CurrentIdp()
{
    return executor.CurrentIdp();
}

template <typename R, typename ... P>
idp_t Agent<R(P...)>::OriginalIdp()
{
    return executor.OriginalIdp();
}

template <typename R, typename ... P>
idp_t Agent<R(P...)>::GetExecutorIdp()
{
    return executor.GetExecutorIdp();
}


/* Mailbox interface */
// template <typename R, typename ... P>
// void Agent<R(P...)>::Send1(idp_t dest, Message const& msg)
// {
//     typedef Mailbox::Send1_action_Mailbox action_type;
//     return hpx::async<action_type>(_mailBox_gid, dest, msg).get();
// }

// template <typename R, typename ... P>
// void Agent<R(P...)>::Send2(std::vector<idp_t> const& dests, Message const& msg)
// {
//     typedef Mailbox::Send2_action_Mailbox action_type;
//     return hpx::async<action_type>(_mailBox_gid, dests, msg).get();
// }

// template <typename R, typename ... P>
// Message Agent<R(P...)>::Receive1()
// {
//     typedef Mailbox::Receive1_action_Mailbox action_type;
//     return hpx::async<action_type>(_mailBox_gid).get();
// }

// template <typename R, typename ... P>
// Message Agent<R(P...)>::Receive2(idp_t source)
// {
//     typedef Mailbox::Receive2_action_Mailbox action_type;
//     return hpx::async<action_type>(_mailBox_gid, source).get();
// }


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
