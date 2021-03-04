#include "cor/resources/group.hpp"


namespace cor {

Group::~Group() = default;

Group::Group(idp_t idp, unsigned int pod_id, std::string const& module) :
    Resource{idp},
    _dynamic_organizer{idp, pod_id, module}
{
    //std::cout << "Criado um componente \"Grupo\", com idp: " << idp << std::endl;
}


/* DynamicOrganizer's interface */
void Group::Join(idp_t idp, std::string const& name) {
    return _dynamic_organizer.Join(idp, name);
}

void Group::Leave(idp_t idp) {
    return _dynamic_organizer.Leave(idp);
}

std::string Group::GetModuleName() {
    return _dynamic_organizer.GetModuleName();
}

std::size_t Group::GetTotalMembers() {
    return _dynamic_organizer.GetTotalMembers();
}

std::vector<idp_t> Group::GetMemberList() {
    return _dynamic_organizer.GetMemberList();
}

idp_t Group::GetIdp1(idm_t idm) {
    return _dynamic_organizer.GetIdp(idm);
}

idp_t Group::GetIdp2(std::string const& name) {
    return _dynamic_organizer.GetIdp(name);
}

idm_t Group::GetIdm1(idp_t idp) {
    return _dynamic_organizer.GetIdm(idp);
}

idm_t Group::GetIdm2(std::string const& name) {
    return _dynamic_organizer.GetIdm(name);
}

idp_t Group::GetDynamicOrganizerIdp() {
    return _dynamic_organizer.GetDynamicOrganizerIdp();
}


}


typedef cor::Group Group;
typedef hpx::components::component<Group> Group_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(Group_type, Group, "Resource");


/* Action registration to interact with DynamicOranizer */
typedef cor::Group::Join_action_Group Join_action_Group;
typedef cor::Group::Leave_action_Group Leave_action_Group;
typedef cor::Group::GetModuleName_action_Group GetModuleName_action_Group;
typedef cor::Group::GetTotalMembers_action_Group GetTotalMembers_action_Group;
typedef cor::Group::GetMemberList_action_Group GetMemberList_action_Group;
typedef cor::Group::GetIdp1_action_Group GetIdp1_action_Group;
typedef cor::Group::GetIdp2_action_Group GetIdp2_action_Group;
typedef cor::Group::GetIdm1_action_Group GetIdm1_action_Group;
typedef cor::Group::GetIdm2_action_Group GetIdm2_action_Group;
typedef cor::Group::GetDynamicOrganizerIdp_action_Group GetDynamicOrganizerIdp_action_Group;

HPX_REGISTER_ACTION(Join_action_Group);
HPX_REGISTER_ACTION(Leave_action_Group);
HPX_REGISTER_ACTION(GetModuleName_action_Group);
HPX_REGISTER_ACTION(GetTotalMembers_action_Group);
HPX_REGISTER_ACTION(GetMemberList_action_Group);
HPX_REGISTER_ACTION(GetIdp1_action_Group);
HPX_REGISTER_ACTION(GetIdp2_action_Group);
HPX_REGISTER_ACTION(GetIdm1_action_Group);
HPX_REGISTER_ACTION(GetIdm2_action_Group);
HPX_REGISTER_ACTION(GetDynamicOrganizerIdp_action_Group);