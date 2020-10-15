// #ifdef COR_AGENT_HPP

// namespace cor {

// template <typename R, typename ... P>
// Agent<R(P...)>::Agent() = default;

// template <typename R, typename ... P>
// Agent<R(P...)>::Agent(idp_t idp, std::function<R(P...)> const& f) :
//     Resource{idp},
//     Executor<R(P...)>{idp, f},
//     Mailbox{idp}
// {}

// template <typename R, typename ... P>
// Agent<R(P...)>::Agent(idp_t idp, std::string const& module, std::string const& function) :
//     Resource{idp},
//     Executor<R(P...)>{idp, module, function},
//     Mailbox{idp}
// {}

// template <typename R, typename ... P>
// Agent<R(P...)>::~Agent() = default;

// template <typename R, typename ... P>
// Agent<R(P...)>::Agent(Agent<R(P...)>&&) noexcept = default;

// template <typename R, typename ... P>
// Agent<R(P...)>& Agent<R(P...)>::operator=(Agent<R(P...)>&&) noexcept = default;

// }

// #endif