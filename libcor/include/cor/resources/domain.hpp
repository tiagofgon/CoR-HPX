#ifndef COR_DOMAIN_HPP
#define COR_DOMAIN_HPP

#include "cor/resources/resource.hpp"
#include "cor/elements/dynamic_organizer.hpp"
#include "cor/elements/container.hpp"

#include <hpx/hpx.hpp>


namespace cor {

struct Domain: public Resource, public hpx::components::component_base<Domain>
{

typedef hpx::components::component_base<Domain>::wrapping_type wrapping_type;
typedef Domain type_holder;
typedef Resource base_type_holder;

protected:
    explicit Domain(idp_t idp, std::string const& module);

public:
    Domain() = delete;
    ~Domain();


    /* DynamicOrganizer's interface */
    void Join(idp_t idp, std::string const& name);
    void Leave(idp_t idp);

    std::string GetModuleName();

    std::size_t GetTotalMembers();
    std::vector<idp_t> GetMemberList();

    idp_t GetIdp1(idm_t idm);
    idp_t GetIdp2(std::string const& name);

    idm_t GetIdm1(idp_t idp);
    idm_t GetIdm2(std::string const& name);

    idp_t GetDynamicOrganizerIdp();

    HPX_DEFINE_COMPONENT_ACTION(Domain, Join, Join_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, Leave, Leave_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetModuleName, GetModuleName_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetTotalMembers, GetTotalMembers_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetMemberList, GetMemberList_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetIdp1, GetIdp1_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetIdp2, GetIdp2_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetIdm1, GetIdm1_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetIdm2, GetIdm2_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetDynamicOrganizerIdp, GetDynamicOrganizerIdp_action_Domain);


    /* Container's interface */
    std::string GetGlobalContext();
    std::string GetLocalContext();
    
    unsigned int GetNumPods();
    unsigned int GetNumDomains();

    std::vector<idp_t> GetPods();
    std::vector<idp_t> GetDomains();

    idp_t GetActiveResourceIdp(size_t id);
    idp_t GetPredecessorIdp(idp_t idp);

    template <typename T>
    std::unique_ptr<T> GetLocalResource(idp_t idp);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, Args ... args);

    template <typename T, typename ... Args>
    idp_t Create(idp_t ctx, std::string const& name, Args ... args);

    template <typename T, typename ... Args>
    idp_t CreateRemote(idp_t ctx, std::string const& name, Args ... args);

    template <typename T>
    std::unique_ptr<T> CreateReference(idp_t idp, idp_t ctx, std::string const& name);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective1(idp_t ctx, std::string const& name, unsigned int total_members, Args ... args);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective2(idp_t active_rsc_idp, idp_t clos, idp_t ctx, std::string const& name, Args ... args);

    template <typename T, typename ... Args>
    auto Run(idp_t idp, Args... args);

    idp_t Spawn(std::string const& context, unsigned int npods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts);

    idp_t GetContainerIdp();

    HPX_DEFINE_COMPONENT_ACTION(Domain, GetGlobalContext, GetGlobalContext_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetLocalContext, GetLocalContext_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetNumPods, GetNumPods_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetNumDomains, GetNumDomains_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetPods, GetPods_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetDomains, GetDomains_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetActiveResourceIdp, GetActiveResourceIdp_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetPredecessorIdp, GetPredecessorIdp_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, Spawn, Spawn_action_Domain);
    HPX_DEFINE_COMPONENT_ACTION(Domain, GetContainerIdp, GetContainerIdp_action_Domain);

    // Actions with template arguments require special
    // type definitions. The simplest way to define such an action type is
    // by deriving from the HPX facility make_action.
    template <typename T>
    struct GetLocalResource_action_Domain
    : hpx::actions::make_action<
        decltype(&Domain::GetLocalResource<T>),
        &Domain::GetLocalResource<T>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateLocal_action_Domain
    : hpx::actions::make_action<
        decltype(&Domain::CreateLocal<T, Args...>),
        &Domain::CreateLocal<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct Create_action_Domain
    : hpx::actions::make_action<
        decltype(&Domain::Create<T, Args...>),
        &Domain::Create<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateRemote_action_Domain
    : hpx::actions::make_action<
        decltype(&Domain::CreateRemote<T, Args...>),
        &Domain::CreateRemote<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateReference_action_Domain
    : hpx::actions::make_action<
        decltype(&Domain::CreateReference<T, Args...>),
        &Domain::CreateReference<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateCollective1_action_Domain
    : hpx::actions::make_action<
        decltype(&Domain::CreateCollective1<T, Args...>),
        &Domain::CreateCollective1<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateCollective2_action_Domain
    : hpx::actions::make_action<
        decltype(&Domain::CreateCollective2<T, Args...>),
        &Domain::CreateCollective2<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct Run_action_Domain
    : hpx::actions::make_action<
        decltype(&Domain::Run<T, Args...>),
        &Domain::Run<T, Args...>
    >::type
    {};


private:
    DynamicOrganizer _dynamic_organizer;
    Container _container;
};

}


#include "cor/resources/domain.tpp"


/* Declaration of actions to interact with DynamicOranizer */
typedef cor::Domain::Join_action_Domain Join_action_Domain;
typedef cor::Domain::Leave_action_Domain Leave_action_Domain;
typedef cor::Domain::GetModuleName_action_Domain GetModuleName_action_Domain;
typedef cor::Domain::GetTotalMembers_action_Domain GetTotalMembers_action_Domain;
typedef cor::Domain::GetMemberList_action_Domain GetMemberList_action_Domain;
typedef cor::Domain::GetIdp1_action_Domain GetIdp1_action_Domain;
typedef cor::Domain::GetIdp2_action_Domain GetIdp2_action_Domain;
typedef cor::Domain::GetIdm1_action_Domain GetIdm1_action_Domain;
typedef cor::Domain::GetIdm2_action_Domain GetIdm2_action_Domain;
typedef cor::Domain::GetDynamicOrganizerIdp_action_Domain GetDynamicOrganizerIdp_action_Domain;

HPX_REGISTER_ACTION_DECLARATION(Join_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(Leave_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetModuleName_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetTotalMembers_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetMemberList_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetIdp1_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetIdp2_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetIdm1_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetIdm2_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetDynamicOrganizerIdp_action_Domain);


/* Declaration of actions to interact with Container */
typedef cor::Domain::GetGlobalContext_action_Domain GetGlobalContext_action_Domain;
typedef cor::Domain::GetLocalContext_action_Domain GetLocalContext_action_Domain;
typedef cor::Domain::GetNumPods_action_Domain GetNumPods_action_Domain;
typedef cor::Domain::GetNumDomains_action_Domain GetNumDomains_action_Domain;
typedef cor::Domain::GetPods_action_Domain GetPods_action_Domain;
typedef cor::Domain::GetDomains_action_Domain GetDomains_action_Domain;
typedef cor::Domain::GetActiveResourceIdp_action_Domain GetActiveResourceIdp_action_Domain;
typedef cor::Domain::GetPredecessorIdp_action_Domain GetPredecessorIdp_action_Domain;
typedef cor::Domain::Spawn_action_Domain Spawn_action_Domain;
typedef cor::Domain::GetContainerIdp_action_Domain GetContainerIdp_action_Domain;

HPX_REGISTER_ACTION_DECLARATION(GetGlobalContext_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetLocalContext_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetNumPods_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetNumDomains_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetPods_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetDomains_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetActiveResourceIdp_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetPredecessorIdp_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(Spawn_action_Domain);
HPX_REGISTER_ACTION_DECLARATION(GetContainerIdp_action_Domain);


#endif
