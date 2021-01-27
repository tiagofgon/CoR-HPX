
#include <hpx/hpx.hpp>
#include "cor/system/pod.hpp"
#include "cor/utils/utils.hpp"
#include "cor/services/mailer.hpp"

#include "cor/resources/domain.hpp"

using namespace dll;

namespace cor {

Pod::Pod(std::string const& id, std::string const& app_group, std::string const& context, unsigned int npods) :
    // _mlr{nullptr},
    _ctrl{nullptr},
    _modules{},
    _active_rscs{}
{
    // _mlr = new Mailer{id, app_group};
    _ctrl = new Controller(id, app_group, context, npods);
}

Pod::~Pod()
{
    // delete _ctrl;
    // delete _mlr;
}

void Pod::Initialize()
{
    // start controller service
    _ctrl->StartService();
}

void Pod::Finalize()
{
    // stop controller services
    _ctrl->StopService();
}

std::string Pod::GetGlobalContext()
{
    return _ctrl->GetGlobalContext();
}

std::string Pod::GetLocalContext()
{
    return _ctrl->GetLocalContext();
}

unsigned int Pod::GetNumPods()
{
    return _ctrl->GetNumPods();
}

unsigned int Pod::GetNumDomains()
{
    return _ctrl->GetNumDomains();
}

std::vector<idp_t> Pod::GetPods()
{
    return _ctrl->GetPods();
}

std::vector<idp_t> Pod::GetDomains()
{
    return _ctrl->GetDomains();
}

idp_t Pod::GetActiveResourceIdp(size_t id) {
    // std::cout << "Thread HPX - GetActiveResourceIdp2: " << id << std::endl;
    idp_t nnn = _active_rscs.at(id);

    return nnn;
}

idp_t Pod::GetDomainIdp1()
{
    return domain_idp;
}

idp_t Pod::GetDomainIdp2(idp_t idp)
{
    return _ctrl->GetDomainIdp(idp);
}

void Pod::setDomainIdp(idp_t idp)
{
    domain_idp = idp;
}

idp_t Pod::GetPredecessorIdp(idp_t idp)
{
    return _ctrl->GetPredecessorIdp(idp);
}

hpx::id_type Pod::GetGidFromIdp(idp_t idp)
{
    return _ctrl->GetGidFromIdp(idp);
}

idp_t Pod::Spawn(std::string const& context, unsigned int npods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
{
    return _ctrl->Spawn(context, npods, parent, module, args, hosts);
}

void Pod::LoadModule(std::string const& module)
{
    // std::unique_lock<std::mutex> lk(_mtx);
    if (_modules.find(module) == _modules.end()) {
        auto dylib = DynamicLoader::LoadDynamicLibrary(module);
        _modules.emplace(utils::get_filename(module), dylib);
    }
}

void Pod::InsertActiveResource(size_t tid, idp_t idp)
{
    std::unique_lock<std::mutex> lk(_mtx);
    if (_active_rscs.find(tid) == _active_rscs.end())
        _active_rscs.emplace(tid, idp);
}

void Pod::RemoveActiveResource(size_t tid)
{
    std::unique_lock<std::mutex> lk(_mtx);
    _active_rscs.erase(tid);
}

void Pod::ChangeActiveResource(size_t tid, idp_t idp)
{
    std::unique_lock<std::mutex> lk(_mtx);
    _active_rscs.at(tid) = idp;
}

idp_t Pod::GetCurrentActiveResource(size_t tid)
{
    std::unique_lock<std::mutex> lk(_mtx);
    return _active_rscs.at(tid);
}

std::string Pod::SearchResource(idp_t idp)
{
    return _ctrl->SearchResource(idp);
}

bool Pod::ContainsResource(idp_t idp)
{
    return _ctrl->ContainsResource(idp);
}


// accessed by Mailbox
// void Pod::SendMessage1(idp_t idp, idp_t dest, Message const& msg)
// {
//     _mlr->SendMessage(idp, dest, msg);
// }

// void Pod::SendMessage2(idp_t idp, std::vector<idp_t> const& dests, Message const& msg)
// {
//     _mlr->SendMessage(idp, dests, msg);
// }

// Message Pod::ReceiveMessage1(idp_t idp)
// {
//     return _mlr->ReceiveMessage(idp);
// }

// Message Pod::ReceiveMessage2(idp_t idp, idp_t source)
// {
//     return _mlr->ReceiveMessage(idp, source);
// }
void Pod::InsertAgentMailbox(idp_t idp, hpx::id_type gid)
{
    return _ctrl->InsertAgentMailbox(idp, gid);
}

hpx::id_type Pod::GetAgentMailbox(idp_t idp)
{
    return _ctrl->GetAgentMailbox(idp);
}





}


typedef cor::Pod Pod;
typedef hpx::components::component<cor::Pod> Pod_type;

HPX_REGISTER_COMPONENT(Pod_type, Pod);


typedef cor::Pod::Initialize_action_pod Initialize_action_pod;
typedef cor::Pod::Finalize_action_pod Finalize_action_pod;
typedef cor::Pod::GetGlobalContext_action_pod GetGlobalContext_action_pod;
typedef cor::Pod::GetLocalContext_action_pod GetLocalContext_action_pod;
typedef cor::Pod::GetNumPods_action_pod GetNumPods_action_pod;
typedef cor::Pod::GetNumDomains_action_pod GetNumDomains_action_pod;
typedef cor::Pod::GetPods_action_pod GetPods_action_pod;
typedef cor::Pod::GetDomains_action_pod GetDomains_action_pod;
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



HPX_REGISTER_ACTION(Initialize_action_pod);
HPX_REGISTER_ACTION(Finalize_action_pod);
HPX_REGISTER_ACTION(GetGlobalContext_action_pod);
HPX_REGISTER_ACTION(GetLocalContext_action_pod);
HPX_REGISTER_ACTION(GetNumPods_action_pod);
HPX_REGISTER_ACTION(GetNumDomains_action_pod);
HPX_REGISTER_ACTION(GetPods_action_pod);
HPX_REGISTER_ACTION(GetDomains_action_pod);
HPX_REGISTER_ACTION(GetActiveResourceIdp_action_pod);
HPX_REGISTER_ACTION(GetPredecessorIdp_action_pod);
HPX_REGISTER_ACTION(GetGidFromIdp_action_pod);
HPX_REGISTER_ACTION(SearchResource_action_pod);
HPX_REGISTER_ACTION(ContainsResource_action_pod);
HPX_REGISTER_ACTION(setDomainIdp_action_pod);
HPX_REGISTER_ACTION(LoadModule_action_pod);
HPX_REGISTER_ACTION(GetDomainIdp1_action_pod);
HPX_REGISTER_ACTION(GetDomainIdp2_action_pod);
HPX_REGISTER_ACTION(InsertActiveResource_action_pod);
HPX_REGISTER_ACTION(RemoveActiveResource_action_pod);
HPX_REGISTER_ACTION(ChangeActiveResource_action_pod);
HPX_REGISTER_ACTION(GetCurrentActiveResource_action_pod);
HPX_REGISTER_ACTION(Spawn_action_pod);
HPX_REGISTER_ACTION(InsertAgentMailbox_action_pod);
HPX_REGISTER_ACTION(GetAgentMailbox_action_pod);
// HPX_REGISTER_ACTION(SendMessage1_action_pod);
// HPX_REGISTER_ACTION(SendMessage2_action_pod);
// HPX_REGISTER_ACTION(ReceiveMessage1_action_pod);
// HPX_REGISTER_ACTION(ReceiveMessage2_action_pod);