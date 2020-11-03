
#include "cor/resources/domain.hpp"

namespace cor {

Domain::Domain() = default;

Domain::Domain(idp_t idp, std::string const& module) :
    base_type(idp),
    _dynamic_organizer{idp, module},
    _container{idp}
{
    std::cout << "Criado um componente \"Domain\", com idp: " << idp << std::endl;
}

Domain::~Domain() = default;

/* DynamicOrganizer interface */
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

idp_t Domain::GetDynamicIdp() {
    return _dynamic_organizer.GetDynamicIdp();
}



/* Container interface */
std::string Domain::GetGlobalContext()
{
    return _container.GetGlobalContext();
}

std::string Domain::GetLocalContext()
{
    return _container.GetLocalContext();
}

unsigned int Domain::GetTotalPods()
{
    return _container.GetTotalPods();
}

unsigned int Domain::GetTotalDomains()
{
    return _container.GetTotalDomains();
}

idp_t Domain::GetActiveResourceIdp(size_t id)
{
    return _container.GetActiveResourceIdp(id);
}

idp_t Domain::GetPredecessorIdp(idp_t idp)
{
    return _container.GetPredecessorIdp(idp);
}

idp_t Domain::Spawn(std::string const& context, unsigned int npods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
{
    return _container.Spawn(context, npods, parent, module, args, hosts);  
}

idp_t Domain::GetContainerIdp()
{
    return _container.GetContainerIdp();  
}


}





typedef cor::Domain Domain;
typedef hpx::components::component<Domain> Domain_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(Domain_type, Domain, "Resource");


// DynamicOranizer actions
typedef cor::Domain::Join_action_Domain Join_action_Domain;
typedef cor::Domain::Leave_action_Domain Leave_action_Domain;
typedef cor::Domain::GetModuleName_action_Domain GetModuleName_action_Domain;
typedef cor::Domain::GetTotalMembers_action_Domain GetTotalMembers_action_Domain;
typedef cor::Domain::GetMemberList_action_Domain GetMemberList_action_Domain;
typedef cor::Domain::GetIdp1_action_Domain GetIdp1_action_Domain;
typedef cor::Domain::GetIdp2_action_Domain GetIdp2_action_Domain;
typedef cor::Domain::GetIdm1_action_Domain GetIdm1_action_Domain;
typedef cor::Domain::GetIdm2_action_Domain GetIdm2_action_Domain;
typedef cor::Domain::GetDynamicIdp_action_Domain GetDynamicIdp_action_Domain;

HPX_REGISTER_ACTION(Join_action_Domain);
HPX_REGISTER_ACTION(Leave_action_Domain);
HPX_REGISTER_ACTION(GetModuleName_action_Domain);
HPX_REGISTER_ACTION(GetTotalMembers_action_Domain);
HPX_REGISTER_ACTION(GetMemberList_action_Domain);
HPX_REGISTER_ACTION(GetIdp1_action_Domain);
HPX_REGISTER_ACTION(GetIdp2_action_Domain);
HPX_REGISTER_ACTION(GetIdm1_action_Domain);
HPX_REGISTER_ACTION(GetIdm2_action_Domain);
HPX_REGISTER_ACTION(GetDynamicIdp_action_Domain);



// Container actions
typedef cor::Domain::GetGlobalContext_action_Domain GetGlobalContext_action_Domain;
typedef cor::Domain::GetLocalContext_action_Domain GetLocalContext_action_Domain;
typedef cor::Domain::GetTotalPods_action_Domain GetTotalPods_action_Domain;
typedef cor::Domain::GetTotalDomains_action_Domain GetTotalDomains_action_Domain;
typedef cor::Domain::GetActiveResourceIdp_action_Domain GetActiveResourceIdp_action_Domain;
typedef cor::Domain::GetPredecessorIdp_action_Domain GetPredecessorIdp_action_Domain;
typedef cor::Domain::Spawn_action_Domain Spawn_action_Domain;
typedef cor::Domain::GetContainerIdp_action_Domain GetContainerIdp_action_Domain;

HPX_REGISTER_ACTION(GetGlobalContext_action_Domain);
HPX_REGISTER_ACTION(GetLocalContext_action_Domain);
HPX_REGISTER_ACTION(GetTotalPods_action_Domain);
HPX_REGISTER_ACTION(GetTotalDomains_action_Domain);
HPX_REGISTER_ACTION(GetActiveResourceIdp_action_Domain);
HPX_REGISTER_ACTION(GetPredecessorIdp_action_Domain);
HPX_REGISTER_ACTION(Spawn_action_Domain);
HPX_REGISTER_ACTION(GetContainerIdp_action_Domain);
