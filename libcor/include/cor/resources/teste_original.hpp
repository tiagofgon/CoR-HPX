#ifndef COR_TESTE_HPP
#define COR_TESTE_HPP

#include <string>

#include "cor/resources/resource.hpp"
#include "cor/elements/container.hpp"
#include "cor/elements/dynamic_organizer.hpp"

namespace cor
{

struct Teste: public hpx::components::abstract_migration_support< hpx::components::component_base<Teste>, Resource >
{

using base_type = hpx::components::abstract_migration_support<
    hpx::components::component_base<Teste>, Resource >;

typedef hpx::components::component_base<Teste>::wrapping_type wrapping_type;
typedef Teste type_holder;
typedef Resource base_type_holder;

typedef int data_type;

friend class ResourceManager;

    Teste();
    explicit Teste(idp_t idp, std::string const& module);
    ~Teste();

    // Components that should be migrated using hpx::migrate<> need to
    // be Serializable and CopyConstructable. Components can be
    // MoveConstructable in which case the serialized data is moved into the
    // component's constructor.
    Teste(Teste&& rhs) :
        base_type(std::move(rhs)),
        variavel(rhs.variavel),
        container(rhs.container),
        dynamic_organizer(rhs.dynamic_organizer)
    {}

    Teste& operator=(Teste&& rhs)
    {
        this->Resource::operator=(std::move(static_cast<Resource&>(rhs)));
        variavel = rhs.variavel;
        container = rhs.container;
        dynamic_organizer = rhs.dynamic_organizer;
        return *this;
    }


    template <typename Archive>
    void serialize(Archive& ar, unsigned version)
    {
        ar & hpx::serialization::base_object<Resource>(*this);
        ar & variavel;
        ar & container;
        ar & dynamic_organizer;
    }




    /* DynamicOrganizer interface */
    void Join(idp_t idp, std::string const& name);
    void Leave(idp_t idp);

    std::string GetModuleName();

    std::size_t GetTotalMembers();
    std::vector<idp_t> GetMemberList();

    // metodos com o mesmo nome é complicado para fazer as action, portanto usei 1 e 2, o que nao altera em anda a API final
    idp_t GetIdp1(idm_t idm);
    idp_t GetIdp2(std::string const& name);

    idm_t GetIdm1(idp_t idp);
    idm_t GetIdm2(std::string const& name);

    idp_t GetDynamicIdp();

    HPX_DEFINE_COMPONENT_ACTION(Teste, Join, Join_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, Leave, Leave_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetModuleName, GetModuleName_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetTotalMembers, GetTotalMembers_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetMemberList, GetMemberList_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetIdp1, GetIdp1_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetIdp2, GetIdp2_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetIdm1, GetIdm1_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetIdm2, GetIdm2_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetDynamicIdp, GetDynamicIdp_action_Teste);


    /* Container interface */
    std::string GetGlobalContext();
    std::string GetLocalContext();

    unsigned int GetTotalPods();
    unsigned int GetTotalDomains();

    idp_t GetActiveResourceIdp(size_t id);
    idp_t GetPredecessorIdp(idp_t idp);

    template <typename T>
    std::unique_ptr<T> GetLocalResource(idp_t idp);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T, typename ... Args>
    idp_t CreateRemote(idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T, typename ... Args>
    idp_t Create(idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T>
    std::unique_ptr<T> CreateReference(idp_t idp, idp_t ctx, std::string const& name);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective1(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective2(idp_t active_rsc_idp, idp_t clos, idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T, typename ... Args>
    void Run(idp_t idp, Args&&... args);

    template <typename T>
    void Wait(idp_t idp);

    template <typename T>
    auto Get(idp_t idp);

    idp_t Spawn(std::string const& context, unsigned int npods, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts);

    idp_t GetContainerIdp();

    HPX_DEFINE_COMPONENT_ACTION(Teste, GetGlobalContext, GetGlobalContext_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetLocalContext, GetLocalContext_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetTotalPods, GetTotalPods_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetTotalDomains, GetTotalDomains_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetActiveResourceIdp, GetActiveResourceIdp_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetPredecessorIdp, GetPredecessorIdp_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, Spawn, Spawn_action_Teste);
    HPX_DEFINE_COMPONENT_ACTION(Teste, GetContainerIdp, GetContainerIdp_action_Teste);

    // Actions with template arguments require special
    // type definitions. The simplest way to define such an action type is
    // by deriving from the HPX facility make_action.
    template <typename T>
    struct GetLocalResource_action_Teste
    : hpx::actions::make_action<
        decltype(&Teste::GetLocalResource<T>),
        &Teste::GetLocalResource<T>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateLocal_action_Teste
    : hpx::actions::make_action<
        decltype(&Teste::CreateLocal<T, Args...>),
        &Teste::CreateLocal<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateRemote_action_Teste
    : hpx::actions::make_action<
        decltype(&Teste::CreateRemote<T, Args...>),
        &Teste::CreateRemote<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct Create_action_Teste
    : hpx::actions::make_action<
        decltype(&Teste::Create<T, Args...>),
        &Teste::Create<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateReference_action_Teste
    : hpx::actions::make_action<
        decltype(&Teste::CreateReference<T, Args...>),
        &Teste::CreateReference<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateCollective1_action_Teste
    : hpx::actions::make_action<
        decltype(&Teste::CreateCollective1<T, Args...>),
        &Teste::CreateCollective1<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateCollective2_action_Teste
    : hpx::actions::make_action<
        decltype(&Teste::CreateCollective2<T, Args...>),
        &Teste::CreateCollective2<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct Run_action_Teste
    : hpx::actions::make_action<
        decltype(&Teste::Run<T, Args...>),
        &Teste::Run<T, Args...>
    >::type
    {};

    template <typename T>
    struct Wait_action_Teste
    : hpx::actions::make_action<
        decltype(&Teste::Wait<T>),
        &Teste::Wait<T>
    >::type
    {};

    template <typename T>
    struct Get_action_Teste
    : hpx::actions::make_action<
        decltype(&Teste::Get<T>),
        &Teste::Get<T>
    >::type
    {};




private:
    int variavel;

    Container container;
    DynamicOrganizer dynamic_organizer;

};


}




#include "cor/resources/teste.tpp"

// DynamicOranizer actions
typedef cor::Teste::Join_action_Teste Join_action_Teste;
typedef cor::Teste::Leave_action_Teste Leave_action_Teste;
typedef cor::Teste::GetModuleName_action_Teste GetModuleName_action_Teste;
typedef cor::Teste::GetTotalMembers_action_Teste GetTotalMembers_action_Teste;
typedef cor::Teste::GetMemberList_action_Teste GetMemberList_action_Teste;
typedef cor::Teste::GetIdp1_action_Teste GetIdp1_action_Teste;
typedef cor::Teste::GetIdp2_action_Teste GetIdp2_action_Teste;
typedef cor::Teste::GetIdm1_action_Teste GetIdm1_action_Teste;
typedef cor::Teste::GetIdm2_action_Teste GetIdm2_action_Teste;
typedef cor::Teste::GetDynamicIdp_action_Teste GetDynamicIdp_action_Teste;

HPX_REGISTER_ACTION_DECLARATION(Join_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(Leave_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetModuleName_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetTotalMembers_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetMemberList_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetIdp1_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetIdp2_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetIdm1_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetIdm2_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetDynamicIdp_action_Teste);



// Container actions
typedef cor::Teste::GetGlobalContext_action_Teste GetGlobalContext_action_Teste;
typedef cor::Teste::GetLocalContext_action_Teste GetLocalContext_action_Teste;
typedef cor::Teste::GetTotalPods_action_Teste GetTotalPods_action_Teste;
typedef cor::Teste::GetTotalDomains_action_Teste GetTotalDomains_action_Teste;
typedef cor::Teste::GetActiveResourceIdp_action_Teste GetActiveResourceIdp_action_Teste;
typedef cor::Teste::GetPredecessorIdp_action_Teste GetPredecessorIdp_action_Teste;
typedef cor::Teste::Spawn_action_Teste Spawn_action_Teste;
typedef cor::Teste::GetContainerIdp_action_Teste GetContainerIdp_action_Teste;

HPX_REGISTER_ACTION_DECLARATION(GetGlobalContext_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetLocalContext_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetTotalPods_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetTotalDomains_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetActiveResourceIdp_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetPredecessorIdp_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(Spawn_action_Teste);
HPX_REGISTER_ACTION_DECLARATION(GetContainerIdp_action_Teste);



// ///////////////////////////////////////////////////////////////////////////////
// struct Teste
//   : hpx::components::migration_support<
//         hpx::components::component_base<Teste>
//     >
// {
//     typedef hpx::components::migration_support<
//             hpx::components::component_base<Teste>
//         > base_type;

//     Teste(int data = 0) : data_(data) {}
//     ~Teste() {}

//     // Components which should be migrated using hpx::migrate<> need to
//     // be Serializable and CopyConstructable. Components can be
//     // MoveConstructable in which case the serialized data is moved into the
//     // component's constructor.
//     Teste(Teste const& rhs)
//       : base_type(rhs), data_(rhs.data_)
//     {}

//     Teste(Teste && rhs)
//       : base_type(std::move(rhs)), data_(rhs.data_)
//     {}

//     Teste& operator=(Teste const & rhs)
//     {
//         data_ = rhs.data_;
//         return *this;
//     }
//     Teste& operator=(Teste && rhs)
//     {
//         data_ = rhs.data_;
//         return *this;
//     }

//     template <typename Archive>
//     void serialize(Archive& ar, unsigned version)
//     {
//         ar & data_;
//     }

// private:
//     int data_;
// };










#endif
