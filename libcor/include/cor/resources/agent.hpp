#ifndef COR_AGENT_HPP
#define COR_AGENT_HPP

#include <functional>
#include <hpx/preprocessor/cat.hpp>

#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#include "cor/resources/resource.hpp"
#include "cor/elements/executor_hpx.hpp"
#include "cor/elements/mailbox_client.hpp"

#include <hpx/hpx.hpp>


namespace cor {

template <typename> struct Agent;

template <typename R, typename ... P>
struct Agent<R(P...)>: public Resource, public hpx::components::component_base<Agent<R(P...)>>
{

// hpx stuff for Resource hierarchy
typedef typename hpx::components::component_base<Agent<R(P...)>>::wrapping_type wrapping_type;
typedef Agent type_holder;
typedef Resource base_type_holder;

protected:
    Agent(idp_t idp, unsigned int pod_id, std::function<R(P...)> const& f);
    Agent(idp_t idp, unsigned int pod_id, std::string const& module, std::string const& function);

public:
    Agent() = delete;
    ~Agent() = default;



    /** Executor's interface **/
    template <typename ... Args>
    hpx::future<R> Run1(Args... args);

    template <typename ... Args>
    hpx::future<R> Run2(Args&&... args);

    void ChangeIdp(idp_t idp);
    void ResumeIdp();

    idp_t CurrentIdp();
    idp_t OriginalIdp();

    idp_t GetExecutorIdp();

    HPX_DEFINE_COMPONENT_ACTION(Agent, ChangeIdp, ChangeIdp_action_Agent);
    HPX_DEFINE_COMPONENT_ACTION(Agent, ResumeIdp, ResumeIdp_action_Agent);
    HPX_DEFINE_COMPONENT_ACTION(Agent, CurrentIdp, CurrentIdp_action_Agent);
    HPX_DEFINE_COMPONENT_ACTION(Agent, OriginalIdp, OriginalIdp_action_Agent);
    HPX_DEFINE_COMPONENT_ACTION(Agent, GetExecutorIdp, GetExecutorIdp_action_Agent);

    template <typename ... Args>
    struct Run1_action_Agent
    : hpx::actions::make_action<
        decltype(&Agent::Run1<Args...>),
        &Agent::Run1<Args...>
    >::type
    {};

    template <typename ... Args>
    struct Run2_action_Agent
    : hpx::actions::make_action<
        decltype(&Agent::Run2<Args...>),
        &Agent::Run2<Args...>
    >::type
    {};



    /** Mailbox's interface **/
    void Send1(idp_t dest, Message const& msg);                          // Unicast
    void Send2(std::vector<idp_t> const& dests, Message const& msg);
    Message Receive1();
    Message Receive2(idp_t source);

    // Contextual Communication
    void Broadcast(idp_t clos, Message const& msg);                     // Broadcast
    void Send3(idm_t rank, idp_t clos, Message const& msg);              // Contextual Unicast
    Message Receive3(idm_t rank, idp_t clos);

    hpx::id_type GetMailboxGid();

    HPX_DEFINE_COMPONENT_ACTION(Agent, Send1, Send1_action_Agent);
    HPX_DEFINE_COMPONENT_ACTION(Agent, Send2, Send2_action_Agent);
    HPX_DEFINE_COMPONENT_ACTION(Agent, Receive1, Receive1_action_Agent);
    HPX_DEFINE_COMPONENT_ACTION(Agent, Receive2, Receive2_action_Agent);
    HPX_DEFINE_COMPONENT_ACTION(Agent, Broadcast, Broadcast_action_Agent);
    HPX_DEFINE_COMPONENT_ACTION(Agent, Send3, Send3_action_Agent);
    HPX_DEFINE_COMPONENT_ACTION(Agent, Receive3, Receive3_action_Agent);
    HPX_DEFINE_COMPONENT_ACTION(Agent, GetMailboxGid, GetMailboxGid_action_Agent);



private:
    Executor<R(P...)> _executor;
    Mailbox_Client _mailBox;
};


}


#include "cor/resources/agent.tpp"


// auxiliary function to join the parameters
#define CONCATENATE(...)                                        \
    BOOST_PP_SEQ_CAT(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
/**/

#define REGISTER_AGENT_DECLARATION(res, ...)                                                              \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::ChangeIdp_action_Agent,                                             \
        HPX_PP_CAT(__Agent_ChangeIdp_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));          \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::ResumeIdp_action_Agent,                                             \
        HPX_PP_CAT(__Agent_ResumeIdp_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));          \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::CurrentIdp_action_Agent,                                            \
        HPX_PP_CAT(__Agent_CurrentIdp_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));         \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::OriginalIdp_action_Agent,                                           \
        HPX_PP_CAT(__Agent_OriginalIdp_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));        \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::GetExecutorIdp_action_Agent,                                        \
        HPX_PP_CAT(__Agent_GetExecutorIdp_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));     \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::Send1_action_Agent,                                                 \
        HPX_PP_CAT(__Agent_Send1_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));              \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::Send2_action_Agent,                                                 \
        HPX_PP_CAT(__Agent_Send2_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));              \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::Receive1_action_Agent,                                              \
        HPX_PP_CAT(__Agent_Receive1_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));           \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::Receive2_action_Agent,                                              \
        HPX_PP_CAT(__Agent_Receive2_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));           \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::Broadcast_action_Agent,                                             \
        HPX_PP_CAT(__Agent_Broadcast_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));          \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::Send3_action_Agent,                                                 \
        HPX_PP_CAT(__Agent_Send3_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));              \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::Receive3_action_Agent,                                              \
        HPX_PP_CAT(__Agent_Receive3_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));           \
    HPX_REGISTER_ACTION_DECLARATION(                                                                      \
        cor::Agent<res(__VA_ARGS__)>::GetMailboxGid_action_Agent,                                         \
        HPX_PP_CAT(__Agent_GetMailboxGid_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__)))); 
/**/

#define REGISTER_AGENT(res, ...)                                                                          \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::ChangeIdp_action_Agent,                                             \
        HPX_PP_CAT(__Agent_ChangeIdp_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));          \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::ResumeIdp_action_Agent,                                             \
        HPX_PP_CAT(__Agent_ResumeIdp_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));          \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::CurrentIdp_action_Agent,                                            \
        HPX_PP_CAT(__Agent_CurrentIdp_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));         \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::OriginalIdp_action_Agent,                                           \
        HPX_PP_CAT(__Agent_OriginalIdp_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));        \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::GetExecutorIdp_action_Agent,                                        \
        HPX_PP_CAT(__Agent_GetExecutorIdp_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));     \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::Send1_action_Agent,                                                 \
        HPX_PP_CAT(__Agent_Send1_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));              \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::Send2_action_Agent,                                                 \
        HPX_PP_CAT(__Agent_Send2_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));              \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::Receive1_action_Agent,                                              \
        HPX_PP_CAT(__Agent_Receive1_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));           \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::Receive2_action_Agent,                                              \
        HPX_PP_CAT(__Agent_Receive2_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));           \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::Broadcast_action_Agent,                                             \
        HPX_PP_CAT(__Agent_Broadcast_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));          \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::Send3_action_Agent,                                                 \
        HPX_PP_CAT(__Agent_Send3_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));              \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::Receive3_action_Agent,                                              \
        HPX_PP_CAT(__Agent_Receive3_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));           \
    HPX_REGISTER_ACTION(                                                                                  \
        cor::Agent<res(__VA_ARGS__)>::GetMailboxGid_action_Agent,                                         \
        HPX_PP_CAT(__Agent_GetMailboxGid_action_Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));      \
  typedef ::hpx::components::component<cor::Agent<res(__VA_ARGS__)>>                                      \
      HPX_PP_CAT(__Agent_type, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__)));                                \
  typedef cor::Agent<res(__VA_ARGS__)>                                                                    \
      HPX_PP_CAT(__Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__)));                                    \
  HPX_REGISTER_DERIVED_COMPONENT_FACTORY(HPX_PP_CAT(__Agent_type, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))), HPX_PP_CAT(__Agent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))), "Resource")
/**/

#endif
