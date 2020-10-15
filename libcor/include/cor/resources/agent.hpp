// #ifndef COR_AGENT_HPP
// #define COR_AGENT_HPP

// #include <functional>

// #include "cor/resources/resource.hpp"
// #include "cor/elements/executor_hpx.hpp"
// #include "cor/elements/mailbox.hpp"

// #include "cereal/types/polymorphic.hpp"

// namespace cor {

// template <typename> class Agent;

// template <typename R, typename ... P>
// class Agent<R(P...)>: public Resource, public Executor<R(P...)>, public Mailbox
// {

// friend class ResourceManager;
// friend class cereal::access;

// public:    
//     ~Agent();

//     Agent(const Agent&) = delete;
//     Agent& operator=(const Agent&) = delete;

//     Agent(Agent&&) noexcept;
//     Agent& operator=(Agent&&) noexcept;

// protected:
//     Agent();

//     Agent(idp_t idp, std::function<R(P...)> const& f);
//     Agent(idp_t idp, std::string const& module, std::string const& function);

// private:
// 	template <typename Archive>
//     void serialize(Archive& ar)
//     {
//         ar(
//             cereal::base_class<Resource>(this),
//             cereal::base_class<Executor<R(P...)>>(this),
//             cereal::base_class<Mailbox>(this)
//         );
//     }

// };

// }

// #include <cereal/archives/binary.hpp>
// #include <cereal/archives/portable_binary.hpp>

// #include "cor/resources/agent.tpp"

// #endif
