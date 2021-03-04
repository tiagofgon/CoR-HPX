#ifndef COR_GROUP_HPP
#define COR_GROUP_HPP

#include "cor/resources/resource.hpp"
#include "cor/elements/dynamic_organizer.hpp"
#include "cor/external/dll/dll.hpp"
#include <hpx/hpx.hpp>

namespace cor {
struct Group: public Resource, public hpx::components::component_base<Group>
{

typedef hpx::components::component_base<Group>::wrapping_type wrapping_type;
typedef Group type_holder;
typedef Resource base_type_holder;

protected:
    explicit Group(idp_t idp, unsigned int pod_id, std::string const& module);

public:
    Group() = delete;
    ~Group();


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

    HPX_DEFINE_COMPONENT_ACTION(Group, Join, Join_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, Leave, Leave_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetModuleName, GetModuleName_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetTotalMembers, GetTotalMembers_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetMemberList, GetMemberList_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetIdp1, GetIdp1_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetIdp2, GetIdp2_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetIdm1, GetIdm1_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetIdm2, GetIdm2_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetDynamicOrganizerIdp, GetDynamicOrganizerIdp_action_Group);


private:
    DynamicOrganizer _dynamic_organizer;
};

}


/* Declaration of actions to interact with DynamicOranizer */
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

HPX_REGISTER_ACTION_DECLARATION(Join_action_Group);
HPX_REGISTER_ACTION_DECLARATION(Leave_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetModuleName_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetTotalMembers_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetMemberList_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetIdp1_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetIdp2_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetIdm1_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetIdm2_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetDynamicOrganizerIdp_action_Group);


#endif
