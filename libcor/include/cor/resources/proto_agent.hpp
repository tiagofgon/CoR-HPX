#ifndef COR_PROTO_AGENT_HPP
#define COR_PROTO_AGENT_HPP

#include <functional>
#include <hpx/preprocessor/cat.hpp>

#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#include "cor/resources/resource.hpp"
#include "cor/elements/executor_hpx.hpp"

#include <hpx/hpx.hpp>


namespace cor {

template <typename> struct ProtoAgent;

template <typename R, typename ... P>
struct ProtoAgent<R(P...)>: public Resource, public hpx::components::component_base<ProtoAgent<R(P...)>>
{

typedef typename hpx::components::component_base<ProtoAgent<R(P...)>>::wrapping_type wrapping_type;
typedef ProtoAgent type_holder;
typedef Resource base_type_holder;

protected:
    ProtoAgent(idp_t idp, std::function<R(P...)> const& f);
    ProtoAgent(idp_t idp, std::string const& module, std::string const& function);

public:
    ProtoAgent() = delete;
    ~ProtoAgent();


    /* Executor interface */
    template <typename ... Args>
    void Run(Args&&... args);

    void Wait();
    R Get();

    void ChangeIdp(idp_t idp);
    void ResumeIdp();

    idp_t CurrentIdp();
    idp_t OriginalIdp();

    idp_t GetExecutorIdp();

    HPX_DEFINE_COMPONENT_ACTION(ProtoAgent, Wait, Wait_action_ProtoAgent);
    HPX_DEFINE_COMPONENT_ACTION(ProtoAgent, Get, Get_action_ProtoAgent) ;
    HPX_DEFINE_COMPONENT_ACTION(ProtoAgent, ChangeIdp, ChangeIdp_action_ProtoAgent);
    HPX_DEFINE_COMPONENT_ACTION(ProtoAgent, ResumeIdp, ResumeIdp_action_ProtoAgent);
    HPX_DEFINE_COMPONENT_ACTION(ProtoAgent, CurrentIdp, CurrentIdp_action_ProtoAgent);
    HPX_DEFINE_COMPONENT_ACTION(ProtoAgent, OriginalIdp, OriginalIdp_action_ProtoAgent);
    HPX_DEFINE_COMPONENT_ACTION(ProtoAgent, GetExecutorIdp, GetExecutorIdp_action_ProtoAgent);

    template <typename ... Args>
    struct Run_action_ProtoAgent
    : hpx::actions::make_action<
        decltype(&ProtoAgent::Run<Args...>),
        &ProtoAgent::Run<Args...>
    >::type
    {};


private:
    Executor<R(P...)> _executor;
};

}

#include "cor/resources/proto_agent.tpp"


// auxiliary function to join the parameters
#define CONCATENATE(...)                                        \
    BOOST_PP_SEQ_CAT(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))     \


#define REGISTER_PROTOAGENT_DECLARATION(res, ...)                                                                   \
    HPX_REGISTER_ACTION_DECLARATION(                                                                                \
        cor::ProtoAgent<res(__VA_ARGS__)>::Wait_action_ProtoAgent,                                                  \
        HPX_PP_CAT(__ProtoAgent_Wait_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));               \
    HPX_REGISTER_ACTION_DECLARATION(                                                                                \
        cor::ProtoAgent<res(__VA_ARGS__)>::Get_action_ProtoAgent,                                                   \
        HPX_PP_CAT(__ProtoAgent_Get_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));                \
    HPX_REGISTER_ACTION_DECLARATION(                                                                                \
        cor::ProtoAgent<res(__VA_ARGS__)>::ChangeIdp_action_ProtoAgent,                                             \
        HPX_PP_CAT(__ProtoAgent_ChangeIdp_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));          \
    HPX_REGISTER_ACTION_DECLARATION(                                                                                \
        cor::ProtoAgent<res(__VA_ARGS__)>::ResumeIdp_action_ProtoAgent,                                             \
        HPX_PP_CAT(__ProtoAgent_ResumeIdp_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));          \
    HPX_REGISTER_ACTION_DECLARATION(                                                                                \
        cor::ProtoAgent<res(__VA_ARGS__)>::CurrentIdp_action_ProtoAgent,                                            \
        HPX_PP_CAT(__ProtoAgent_CurrentIdp_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));         \
    HPX_REGISTER_ACTION_DECLARATION(                                                                                \
        cor::ProtoAgent<res(__VA_ARGS__)>::OriginalIdp_action_ProtoAgent,                                           \
        HPX_PP_CAT(__ProtoAgent_OriginalIdp_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));        \
    HPX_REGISTER_ACTION_DECLARATION(                                                                                \
        cor::ProtoAgent<res(__VA_ARGS__)>::GetExecutorIdp_action_ProtoAgent,                                        \
        HPX_PP_CAT(__ProtoAgent_GetExecutorIdp_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));     \


#define REGISTER_PROTOAGENT(res, ...)                                                                               \
    HPX_REGISTER_ACTION(                                                                                            \
        cor::ProtoAgent<res(__VA_ARGS__)>::Wait_action_ProtoAgent,                                                  \
        HPX_PP_CAT(__ProtoAgent_Wait_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));               \
    HPX_REGISTER_ACTION(                                                                                            \
        cor::ProtoAgent<res(__VA_ARGS__)>::Get_action_ProtoAgent,                                                   \
        HPX_PP_CAT(__ProtoAgent_Get_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));                \
    HPX_REGISTER_ACTION(                                                                                            \
        cor::ProtoAgent<res(__VA_ARGS__)>::ChangeIdp_action_ProtoAgent,                                             \
        HPX_PP_CAT(__ProtoAgent_ChangeIdp_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));          \
    HPX_REGISTER_ACTION(                                                                                            \
        cor::ProtoAgent<res(__VA_ARGS__)>::ResumeIdp_action_ProtoAgent,                                             \
        HPX_PP_CAT(__ProtoAgent_ResumeIdp_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));          \
    HPX_REGISTER_ACTION(                                                                                            \
        cor::ProtoAgent<res(__VA_ARGS__)>::CurrentIdp_action_ProtoAgent,                                            \
        HPX_PP_CAT(__ProtoAgent_CurrentIdp_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));         \
    HPX_REGISTER_ACTION(                                                                                            \
        cor::ProtoAgent<res(__VA_ARGS__)>::OriginalIdp_action_ProtoAgent,                                           \
        HPX_PP_CAT(__ProtoAgent_OriginalIdp_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));        \
    HPX_REGISTER_ACTION(                                                                                            \
        cor::ProtoAgent<res(__VA_ARGS__)>::GetExecutorIdp_action_ProtoAgent,                                        \
        HPX_PP_CAT(__ProtoAgent_GetExecutorIdp_action_ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))));     \
  typedef ::hpx::components::component<cor::ProtoAgent<res(__VA_ARGS__)>>                                           \
      HPX_PP_CAT(__ProtoAgent_type, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__)));                                     \
  typedef cor::ProtoAgent<res(__VA_ARGS__)>                                                                         \
      HPX_PP_CAT(__ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__)));                                         \
  HPX_REGISTER_DERIVED_COMPONENT_FACTORY(HPX_PP_CAT(__ProtoAgent_type, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))), HPX_PP_CAT(__ProtoAgent_, HPX_PP_CAT(res, CONCATENATE(__VA_ARGS__))), "Resource")               \


#endif
