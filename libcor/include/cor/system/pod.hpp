#ifndef COR_POD_HPP
#define COR_POD_HPP


#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>

#include "cor/system/macros.hpp"
#include "cor/external/dll/dll.hpp"
#include "cor/message.hpp"
#include "cor/services/controller.hpp"

#include <hpx/hpx.hpp>


namespace cor {

//class Mailer;

class Pod : public hpx::components::component_base<Pod>
// class Pod : public hpx::components::locking_hook<hpx::components::component_base<Pod>>
{

friend class Container;
friend class DynamicOrganizer;
friend class StaticOrganizer;
template <typename> friend class Value;
template <typename> friend class Executor;

public:
    explicit Pod(std::string const& id, std::string const& app_group, std::string const& context, unsigned int npods);

    ~Pod();

    void Initialize();
    void Finalize();

    std::string GetGlobalContext();
    std::string GetLocalContext();

    unsigned int GetTotalPods();
    unsigned int GetTotalDomains();

    idp_t GetActiveResourceIdp(size_t id);
    
    idp_t GetDomainIdp1();

    // // not global function, only local domain, PARA ACABAR
    idp_t GetDomainIdp2(idp_t idp);

    // not global function, only local resources predecessor
    idp_t GetPredecessorIdp(idp_t idp);

    hpx::id_type GetGidFromIdp(idp_t idp);

    template <typename T>
    std::unique_ptr<T> GetLocalResource(idp_t idp);

    template <typename T>
    std::unique_ptr<T> CreateLocal_agent(idp_t ctx, std::string const& name, hpx::function<void(void*)> const& func);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T, typename ... Args>
    idp_t Create(idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T, typename ... Args>
    idp_t CreateRemote(idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args);

    template <typename T>
    std::unique_ptr<T> CreateReference(idp_t idp, idp_t ctx, std::string const& name);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective1(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective2(idm_t rank, idp_t comm, idp_t ctx, std::string const& name, Args&& ... args);

    idp_t Spawn(std::string const& context, unsigned int npods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts);

    
    // to remove
    void Debug();

    Pod() = delete;
    Pod(const Pod&) = delete;
    Pod& operator=(const Pod&) = delete;
    Pod(Pod&&) = delete;
    Pod& operator=(Pod&&) = delete;



    HPX_DEFINE_COMPONENT_ACTION(Pod, Initialize, Initialize_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, Finalize, Finalize_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, GetGlobalContext, GetGlobalContext_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, GetLocalContext, GetLocalContext_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, GetTotalPods, GetTotalPods_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, GetTotalDomains, GetTotalDomains_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, GetActiveResourceIdp, GetActiveResourceIdp_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, GetPredecessorIdp, GetPredecessorIdp_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, GetGidFromIdp, GetGidFromIdp_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, GetDomainIdp1, GetDomainIdp1_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, GetDomainIdp2, GetDomainIdp2_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, Spawn, Spawn_action_pod);

    template <typename T>
    struct GetLocalResource_action_pod 
    : hpx::actions::make_action<
        decltype(&Pod::GetLocalResource<T>),
        &Pod::GetLocalResource<T>
    >::type
    {};

    template <typename T>
    struct CreateLocal_agent_action_pod 
    : hpx::actions::make_action<
        decltype(&Pod::CreateLocal_agent<T>),
        &Pod::CreateLocal_agent<T>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateLocal_action_pod 
    : hpx::actions::make_action<
        decltype(&Pod::CreateLocal<T, Args...>),
        &Pod::CreateLocal<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct Create_action_pod 
    : hpx::actions::make_action<
        decltype(&Pod::Create<T, Args...>),
        &Pod::Create<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateRemote_action_pod 
    : hpx::actions::make_action<
        decltype(&Pod::CreateRemote<T, Args...>),
        &Pod::CreateRemote<T, Args...>
    >::type
    {};

    template <typename T>
    struct CreateReference_action_pod 
    : hpx::actions::make_action<
        decltype(&Pod::CreateReference<T>),
        &Pod::CreateReference<T>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateCollective1_action_pod 
    : hpx::actions::make_action<
        decltype(&Pod::CreateCollective1<T, Args...>),
        &Pod::CreateCollective1<T, Args...>
    >::type
    {};

    template <typename T, typename ... Args>
    struct CreateCollective2_action_pod 
    : hpx::actions::make_action<
        decltype(&Pod::CreateCollective2<T, Args...>),
        &Pod::CreateCollective2<T, Args...>
    >::type
    {};


protected:
    // accessed by Container
    std::string SearchResource(idp_t idp);
    bool ContainsResource(idp_t idp);
    void setDomainIdp(idp_t idp);

    // accessed by Organizer
    void LoadModule(std::string const& module);

    // accessed by Executor
    template <typename T>
    std::function<T> LoadFunction(std::string const& module, std::string const& function);

    void InsertActiveResource(size_t tid, idp_t idp);
    void RemoveActiveResource(size_t tid);
    void ChangeActiveResource(size_t tid, idp_t idp);
    idp_t GetCurrentActiveResource(size_t tid);


    // accessed by Mailbox
    void InsertAgentMailbox(idp_t idp, hpx::id_type gid);
    hpx::id_type GetAgentMailbox(idp_t idp);



    public:
    HPX_DEFINE_COMPONENT_ACTION(Pod, SearchResource, SearchResource_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, ContainsResource, ContainsResource_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, setDomainIdp, setDomainIdp_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, LoadModule, LoadModule_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, InsertActiveResource, InsertActiveResource_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, RemoveActiveResource, RemoveActiveResource_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, ChangeActiveResource, ChangeActiveResource_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, GetCurrentActiveResource, GetCurrentActiveResource_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, InsertAgentMailbox, InsertAgentMailbox_action_pod);
    HPX_DEFINE_COMPONENT_ACTION(Pod, GetAgentMailbox, GetAgentMailbox_action_pod);

    // HPX_DEFINE_COMPONENT_ACTION(Pod, SendMessage1, SendMessage1_action_pod);
    // HPX_DEFINE_COMPONENT_ACTION(Pod, SendMessage2, SendMessage2_action_pod);
    // HPX_DEFINE_COMPONENT_ACTION(Pod, ReceiveMessage1, ReceiveMessage1_action_pod);
    // HPX_DEFINE_COMPONENT_ACTION(Pod, ReceiveMessage2, ReceiveMessage2_action_pod);

    template <typename T>
    struct LoadFunction_action_pod 
    : hpx::actions::make_action<
        decltype(&Pod::LoadFunction<T>),
        &Pod::LoadFunction<T>
    >::type
    {};


private:
    // std::shared_ptr<Controller_new> _ctrl_new;
    // Mailer *_mlr;
    Controller *_ctrl;

    std::map<std::string, dll::DynamicLibrary*> _modules;
    std::map<size_t, idp_t> _active_rscs;
    std::mutex _mtx;

    idp_t domain_idp;

};

}

#include "cor/system/pod.tpp"


typedef cor::Pod::Initialize_action_pod Initialize_action_pod;
typedef cor::Pod::Finalize_action_pod Finalize_action_pod;
typedef cor::Pod::GetGlobalContext_action_pod GetGlobalContext_action_pod;
typedef cor::Pod::GetLocalContext_action_pod GetLocalContext_action_pod;
typedef cor::Pod::GetTotalPods_action_pod GetTotalPods_action_pod;
typedef cor::Pod::GetTotalDomains_action_pod GetTotalDomains_action_pod;
typedef cor::Pod::GetActiveResourceIdp_action_pod GetActiveResourceIdp_action_pod;
typedef cor::Pod::GetPredecessorIdp_action_pod GetPredecessorIdp_action_pod;
typedef cor::Pod::GetGidFromIdp_action_pod GetGidFromIdp_action_pod;
typedef cor::Pod::SearchResource_action_pod SearchResource_action_pod;
typedef cor::Pod::ContainsResource_action_pod ContainsResource_action_pod;
typedef cor::Pod::setDomainIdp_action_pod setDomainIdp_action_pod;
typedef cor::Pod::LoadModule_action_pod LoadModule_action_pod;
typedef cor::Pod::GetDomainIdp1_action_pod GetDomainIdp1_action_pod;
typedef cor::Pod::GetDomainIdp2_action_pod GetDomainIdp2_action_pod;
typedef cor::Pod::InsertActiveResource_action_pod InsertActiveResource_action_pod;
typedef cor::Pod::RemoveActiveResource_action_pod RemoveActiveResource_action_pod;
typedef cor::Pod::ChangeActiveResource_action_pod ChangeActiveResource_action_pod;
typedef cor::Pod::GetCurrentActiveResource_action_pod GetCurrentActiveResource_action_pod;
typedef cor::Pod::Spawn_action_pod Spawn_action_pod;
typedef cor::Pod::InsertAgentMailbox_action_pod InsertAgentMailbox_action_pod;
typedef cor::Pod::GetAgentMailbox_action_pod GetAgentMailbox_action_pod;
// typedef cor::Pod::SendMessage1_action_pod SendMessage1_action_pod;
// typedef cor::Pod::SendMessage2_action_pod SendMessage2_action_pod;
// typedef cor::Pod::ReceiveMessage1_action_pod ReceiveMessage1_action_pod;
// typedef cor::Pod::ReceiveMessage2_action_pod ReceiveMessage2_action_pod;


HPX_REGISTER_ACTION_DECLARATION(Initialize_action_pod);
HPX_REGISTER_ACTION_DECLARATION(Finalize_action_pod);
HPX_REGISTER_ACTION_DECLARATION(GetGlobalContext_action_pod);
HPX_REGISTER_ACTION_DECLARATION(GetLocalContext_action_pod);
HPX_REGISTER_ACTION_DECLARATION(GetTotalPods_action_pod);
HPX_REGISTER_ACTION_DECLARATION(GetTotalDomains_action_pod);
HPX_REGISTER_ACTION_DECLARATION(GetActiveResourceIdp_action_pod);
HPX_REGISTER_ACTION_DECLARATION(GetPredecessorIdp_action_pod);
HPX_REGISTER_ACTION_DECLARATION(GetGidFromIdp_action_pod);
HPX_REGISTER_ACTION_DECLARATION(SearchResource_action_pod);
HPX_REGISTER_ACTION_DECLARATION(ContainsResource_action_pod);
HPX_REGISTER_ACTION_DECLARATION(setDomainIdp_action_pod);
HPX_REGISTER_ACTION_DECLARATION(LoadModule_action_pod);
HPX_REGISTER_ACTION_DECLARATION(GetDomainIdp1_action_pod);
HPX_REGISTER_ACTION_DECLARATION(GetDomainIdp2_action_pod);
HPX_REGISTER_ACTION_DECLARATION(InsertActiveResource_action_pod);
HPX_REGISTER_ACTION_DECLARATION(RemoveActiveResource_action_pod);
HPX_REGISTER_ACTION_DECLARATION(ChangeActiveResource_action_pod);
HPX_REGISTER_ACTION_DECLARATION(GetCurrentActiveResource_action_pod);
HPX_REGISTER_ACTION_DECLARATION(Spawn_action_pod);
HPX_REGISTER_ACTION_DECLARATION(InsertAgentMailbox_action_pod);
HPX_REGISTER_ACTION_DECLARATION(GetAgentMailbox_action_pod);

// HPX_REGISTER_ACTION_DECLARATION(SendMessage1_action_pod);
// HPX_REGISTER_ACTION_DECLARATION(SendMessage2_action_pod);
// HPX_REGISTER_ACTION_DECLARATION(ReceiveMessage1_action_pod);
// HPX_REGISTER_ACTION_DECLARATION(ReceiveMessage2_action_pod);


#endif
