
#include "cor/resources/domain.hpp"


namespace cor {

Domain::~Domain() = default;

Domain::Domain(idp_t idp, unsigned int pod_id, std::string const& module) :
    Resource{idp},
    _dynamic_organizer{idp, pod_id, module},
    _container{idp, pod_id},
    _pod_id{pod_id}
{
    // std::cout << "Criado um componente \"Domain\", com idp: " << idp << "com pod_id: " << pod_id << std::endl;
}



/* DynamicOrganizer's interface */
void Domain::Join(idp_t idp, std::string const& name) {
    return _dynamic_organizer.Join(idp, name);
}

void Domain::Leave(idp_t idp) {
    return _dynamic_organizer.Leave(idp);
}

std::string Domain::GetModuleName() {
    return _dynamic_organizer.GetModuleName();
}

std::size_t Domain::GetTotalMembers() {
    return _dynamic_organizer.GetTotalMembers();
}

std::vector<idp_t> Domain::GetMemberList() {
    return _dynamic_organizer.GetMemberList();
}

idp_t Domain::GetIdp1(idm_t idm) {
    return _dynamic_organizer.GetIdp(idm);
}

idp_t Domain::GetIdp2(std::string const& name) {
    return _dynamic_organizer.GetIdp(name);
}

idm_t Domain::GetIdm1(idp_t idp) {
    return _dynamic_organizer.GetIdm(idp);
}

idm_t Domain::GetIdm2(std::string const& name) {
    return _dynamic_organizer.GetIdm(name);
}

idp_t Domain::GetDynamicOrganizerIdp() {
    return _dynamic_organizer.GetDynamicOrganizerIdp();
}


/* Container's interface */
std::string Domain::GetGlobalContext()
{
    return _container.GetGlobalContext();
}

std::string Domain::GetLocalContext()
{
    return _container.GetLocalContext();
}

unsigned int Domain::GetNumPods()
{
    return _container.GetNumPods();
}

unsigned int Domain::GetNumDomains()
{
    return _container.GetNumDomains();
}

std::vector<idp_t> Domain::GetPods()
{
    return _container.GetPods();
}

std::vector<idp_t> Domain::GetDomains()
{
    return _container.GetDomains();
}

idp_t Domain::GetActiveResourceIdp(size_t id)
{
    return _container.GetActiveResourceIdp(id);
}

idp_t Domain::GetPredecessorIdp(idp_t idp)
{
    return _container.GetPredecessorIdp(idp);
}

idp_t Domain::Spawn(std::string const& context, unsigned int npods, unsigned int total_pods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
{
    return _container.Spawn(context, npods, total_pods, parent, module, args, hosts);  
}

idp_t Domain::GetContainerIdp()
{
    return _container.GetContainerIdp();  
}


/* Container's interface */
unsigned int Domain::GetPodId()
{
    return _pod_id;
}


}


typedef cor::Domain Domain;
typedef hpx::components::component<Domain> Domain_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(Domain_type, Domain, "Resource");


/* Action registration to interact with DynamicOranizer */
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

HPX_REGISTER_ACTION(Join_action_Domain);
HPX_REGISTER_ACTION(Leave_action_Domain);
HPX_REGISTER_ACTION(GetModuleName_action_Domain);
HPX_REGISTER_ACTION(GetTotalMembers_action_Domain);
HPX_REGISTER_ACTION(GetMemberList_action_Domain);
HPX_REGISTER_ACTION(GetIdp1_action_Domain);
HPX_REGISTER_ACTION(GetIdp2_action_Domain);
HPX_REGISTER_ACTION(GetIdm1_action_Domain);
HPX_REGISTER_ACTION(GetIdm2_action_Domain);
HPX_REGISTER_ACTION(GetDynamicOrganizerIdp_action_Domain);


/* Action registration to interact with Container */
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

HPX_REGISTER_ACTION(GetGlobalContext_action_Domain);
HPX_REGISTER_ACTION(GetLocalContext_action_Domain);
HPX_REGISTER_ACTION(GetNumPods_action_Domain);
HPX_REGISTER_ACTION(GetNumDomains_action_Domain);
HPX_REGISTER_ACTION(GetPods_action_Domain);
HPX_REGISTER_ACTION(GetDomains_action_Domain);
HPX_REGISTER_ACTION(GetActiveResourceIdp_action_Domain);
HPX_REGISTER_ACTION(GetPredecessorIdp_action_Domain);
HPX_REGISTER_ACTION(Spawn_action_Domain);
HPX_REGISTER_ACTION(GetContainerIdp_action_Domain);


/* Declaration of actions to interact with Domain */
typedef cor::Domain::GetPodId_action_Domain GetPodId_action_Domain;
HPX_REGISTER_ACTION(GetPodId_action_Domain);