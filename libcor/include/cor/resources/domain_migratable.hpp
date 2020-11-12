// #ifndef COR_DOMAIN_HPP
// #define COR_DOMAIN_HPP

// #include <string>

// #include "cor/resources/resource.hpp"
// #include "cor/elements/dynamic_organizer.hpp"
// #include "cor/elements/container.hpp"

// #include <hpx/hpx.hpp>

// namespace cor {

// struct Domain: public hpx::components::abstract_migration_support< hpx::components::component_base<Domain>, Resource >
// {

// using base_type = hpx::components::abstract_migration_support<
//     hpx::components::component_base<Domain>, Resource >;

// typedef hpx::components::component_base<Domain>::wrapping_type wrapping_type;
// typedef Domain type_holder;
// typedef Resource base_type_holder;
    
// protected:
//     explicit Domain(idp_t idp, std::string const& module);

// public:
//     Domain();
//     ~Domain();

//     // Components that should be migrated using hpx::migrate<> need to
//     // be Serializable and CopyConstructable. Components can be
//     // MoveConstructable in which case the serialized data is moved into the
//     // component's constructor.
//     Domain(Domain&& rhs) :
//         base_type(std::move(rhs)),
//         _dynamic_organizer(rhs._dynamic_organizer),
//         _container(rhs._container)
//     {}

//     Domain& operator=(Domain&& rhs)
//     {
//         this->Resource::operator=(std::move(static_cast<Resource&>(rhs)));
//         _dynamic_organizer = rhs._dynamic_organizer;
//         _container = rhs._container;
//         return *this;
//     }

//     /* DynamicOrganizer interface */
//     void Join(idp_t idp, std::string const& name);
//     void Leave(idp_t idp);

//     std::string GetModuleName();

//     std::size_t GetTotalMembers();
//     std::vector<idp_t> GetMemberList();

//     // metodos com o mesmo nome é complicado para fazer as action, portanto usei 1 e 2, o que nao altera em anda a API final
//     idp_t GetIdp1(idm_t idm);
//     idp_t GetIdp2(std::string const& name);

//     idm_t GetIdm1(idp_t idp);
//     idm_t GetIdm2(std::string const& name);

//     idp_t GetDynamicIdp();

//     HPX_DEFINE_COMPONENT_ACTION(Domain, Join, Join_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, Leave, Leave_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetModuleName, GetModuleName_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetTotalMembers, GetTotalMembers_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetMemberList, GetMemberList_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetIdp1, GetIdp1_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetIdp2, GetIdp2_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetIdm1, GetIdm1_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetIdm2, GetIdm2_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetDynamicIdp, GetDynamicIdp_action_Domain);


//     /* Container interface */
//     std::string GetGlobalContext();
//     std::string GetLocalContext();

//     unsigned int GetTotalPods();
//     unsigned int GetTotalDomains();

//     idp_t GetActiveResourceIdp(size_t id);
//     idp_t GetPredecessorIdp(idp_t idp);

//     template <typename T>
//     std::unique_ptr<T> GetLocalResource(idp_t idp);

//     template <typename T, typename ... Args>
//     std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, Args&& ... args);

//     template <typename T, typename ... Args>
//     idp_t CreateRemote(idp_t ctx, std::string const& name, Args&& ... args);

//     template <typename T, typename ... Args>
//     idp_t Create(idp_t ctx, std::string const& name, Args&& ... args);

//     template <typename T>
//     std::unique_ptr<T> CreateReference(idp_t idp, idp_t ctx, std::string const& name);

//     template <typename T, typename ... Args>
//     std::unique_ptr<T> CreateCollective1(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args);

//     template <typename T, typename ... Args>
//     std::unique_ptr<T> CreateCollective2(idp_t active_rsc_idp, idp_t clos, idp_t ctx, std::string const& name, Args&& ... args);

//     template <typename T, typename ... Args>
//     void Run(idp_t idp, Args&&... args);

//     template <typename T>
//     void Wait(idp_t idp);

//     template <typename T>
//     auto Get(idp_t idp);

//     idp_t Spawn(std::string const& context, unsigned int npods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts);

//     idp_t GetContainerIdp();


//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetGlobalContext, GetGlobalContext_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetLocalContext, GetLocalContext_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetTotalPods, GetTotalPods_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetTotalDomains, GetTotalDomains_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetActiveResourceIdp, GetActiveResourceIdp_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetPredecessorIdp, GetPredecessorIdp_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, Spawn, Spawn_action_Domain);
//     HPX_DEFINE_COMPONENT_ACTION(Domain, GetContainerIdp, GetContainerIdp_action_Domain);

//     // Actions with template arguments require special
//     // type definitions. The simplest way to define such an action type is
//     // by deriving from the HPX facility make_action.
//     template <typename T>
//     struct GetLocalResource_action_Domain
//     : hpx::actions::make_action<
//         decltype(&Domain::GetLocalResource<T>),
//         &Domain::GetLocalResource<T>
//     >::type
//     {};

//     template <typename T, typename ... Args>
//     struct CreateLocal_action_Domain
//     : hpx::actions::make_action<
//         decltype(&Domain::CreateLocal<T, Args...>),
//         &Domain::CreateLocal<T, Args...>
//     >::type
//     {};

//     template <typename T, typename ... Args>
//     struct CreateRemote_action_Domain
//     : hpx::actions::make_action<
//         decltype(&Domain::CreateRemote<T, Args...>),
//         &Domain::CreateRemote<T, Args...>
//     >::type
//     {};

//     template <typename T, typename ... Args>
//     struct Create_action_Domain
//     : hpx::actions::make_action<
//         decltype(&Domain::Create<T, Args...>),
//         &Domain::Create<T, Args...>
//     >::type
//     {};

//     template <typename T, typename ... Args>
//     struct CreateReference_action_Domain
//     : hpx::actions::make_action<
//         decltype(&Domain::CreateReference<T, Args...>),
//         &Domain::CreateReference<T, Args...>
//     >::type
//     {};

//     template <typename T, typename ... Args>
//     struct CreateCollective1_action_Domain
//     : hpx::actions::make_action<
//         decltype(&Domain::CreateCollective1<T, Args...>),
//         &Domain::CreateCollective1<T, Args...>
//     >::type
//     {};

//     template <typename T, typename ... Args>
//     struct CreateCollective2_action_Domain
//     : hpx::actions::make_action<
//         decltype(&Domain::CreateCollective2<T, Args...>),
//         &Domain::CreateCollective2<T, Args...>
//     >::type
//     {};

//     template <typename T, typename ... Args>
//     struct Run_action_Domain
//     : hpx::actions::make_action<
//         decltype(&Domain::Run<T, Args...>),
//         &Domain::Run<T, Args...>
//     >::type
//     {};

//     template <typename T>
//     struct Wait_action_Domain
//     : hpx::actions::make_action<
//         decltype(&Domain::Wait<T>),
//         &Domain::Wait<T>
//     >::type
//     {};

//     template <typename T>
//     struct Get_action_Domain
//     : hpx::actions::make_action<
//         decltype(&Domain::Get<T>),
//         &Domain::Get<T>
//     >::type
//     {};

//     template <typename Archive>
//     void serialize(Archive& ar, unsigned version)
//     {
//         ar & hpx::serialization::base_object<Resource>(*this);
//         ar & _dynamic_organizer;
//         ar & _container;
//     }

// private:
//     DynamicOrganizer _dynamic_organizer;
//     Container _container;

// };

// }


// #include "cor/resources/domain.tpp"

// // DynamicOranizer actions
// typedef cor::Domain::Join_action_Domain Join_action_Domain;
// typedef cor::Domain::Leave_action_Domain Leave_action_Domain;
// typedef cor::Domain::GetModuleName_action_Domain GetModuleName_action_Domain;
// typedef cor::Domain::GetTotalMembers_action_Domain GetTotalMembers_action_Domain;
// typedef cor::Domain::GetMemberList_action_Domain GetMemberList_action_Domain;
// typedef cor::Domain::GetIdp1_action_Domain GetIdp1_action_Domain;
// typedef cor::Domain::GetIdp2_action_Domain GetIdp2_action_Domain;
// typedef cor::Domain::GetIdm1_action_Domain GetIdm1_action_Domain;
// typedef cor::Domain::GetIdm2_action_Domain GetIdm2_action_Domain;
// typedef cor::Domain::GetDynamicIdp_action_Domain GetDynamicIdp_action_Domain;

// HPX_REGISTER_ACTION_DECLARATION(Join_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(Leave_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetModuleName_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetTotalMembers_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetMemberList_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetIdp1_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetIdp2_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetIdm1_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetIdm2_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetDynamicIdp_action_Domain);



// // Container actions
// typedef cor::Domain::GetGlobalContext_action_Domain GetGlobalContext_action_Domain;
// typedef cor::Domain::GetLocalContext_action_Domain GetLocalContext_action_Domain;
// typedef cor::Domain::GetTotalPods_action_Domain GetTotalPods_action_Domain;
// typedef cor::Domain::GetTotalDomains_action_Domain GetTotalDomains_action_Domain;
// typedef cor::Domain::GetActiveResourceIdp_action_Domain GetActiveResourceIdp_action_Domain;
// typedef cor::Domain::GetPredecessorIdp_action_Domain GetPredecessorIdp_action_Domain;
// typedef cor::Domain::Spawn_action_Domain Spawn_action_Domain;
// typedef cor::Domain::GetContainerIdp_action_Domain GetContainerIdp_action_Domain;

// HPX_REGISTER_ACTION_DECLARATION(GetGlobalContext_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetLocalContext_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetTotalPods_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetTotalDomains_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetActiveResourceIdp_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetPredecessorIdp_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(Spawn_action_Domain);
// HPX_REGISTER_ACTION_DECLARATION(GetContainerIdp_action_Domain);


// #endif
