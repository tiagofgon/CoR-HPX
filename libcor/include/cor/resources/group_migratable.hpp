#ifndef COR_GROUP_HPP
#define COR_GROUP_HPP

#include <string>

#include "cor/resources/resource.hpp"
#include "cor/elements/dynamic_organizer.hpp"

namespace cor {

// struct Group: public Resource, public hpx::components::component_base<Group>
struct Group: public hpx::components::abstract_migration_support< hpx::components::component_base<Group>, Resource >
{

using base_type = hpx::components::abstract_migration_support<
    hpx::components::component_base<Group>, Resource >;

typedef hpx::components::component_base<Group>::wrapping_type wrapping_type;
typedef Group type_holder;
typedef Resource base_type_holder;

protected:
    explicit Group(idp_t idp, std::string const& module);

public:
    ~Group();
    Group();

    // Components that should be migrated using hpx::migrate<> need to
    // be Serializable and CopyConstructable. Components can be
    // MoveConstructable in which case the serialized data is moved into the
    // component's constructor.
    Group(Group&& rhs) :
        base_type(std::move(rhs)),
        _dynamic_organizer(rhs._dynamic_organizer)
    {}

    Group& operator=(Group&& rhs)
    {
        this->Resource::operator=(std::move(static_cast<Resource&>(rhs)));
        _dynamic_organizer = rhs._dynamic_organizer;
        return *this;
    }

    /* DynamicOrganizer interface */
    void Join(idp_t idp, std::string const& name);
    void Leave(idp_t idp);

    std::string GetModuleName();
    std::size_t GetTotalMembers();
    std::vector<idp_t> GetMemberList();

    idp_t GetIdp1(idm_t idm);
    idp_t GetIdp2(std::string const& name);

    idm_t GetIdm1(idp_t idp);
    idm_t GetIdm2(std::string const& name);

    idp_t GetDynamicIdp();

    HPX_DEFINE_COMPONENT_ACTION(Group, Join, Join_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, Leave, Leave_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetModuleName, GetModuleName_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetTotalMembers, GetTotalMembers_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetMemberList, GetMemberList_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetIdp1, GetIdp1_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetIdp2, GetIdp2_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetIdm1, GetIdm1_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetIdm2, GetIdm2_action_Group);
    HPX_DEFINE_COMPONENT_ACTION(Group, GetDynamicIdp, GetDynamicIdp_action_Group);

    template <typename Archive>
    void serialize(Archive& ar, unsigned version)
    {
        ar & hpx::serialization::base_object<Resource>(*this);
        ar & _dynamic_organizer;
    }

private:
    DynamicOrganizer _dynamic_organizer;

};

}


// DynamicOranizer actions
typedef cor::Group::Join_action_Group Join_action_Group;
typedef cor::Group::Leave_action_Group Leave_action_Group;
typedef cor::Group::GetModuleName_action_Group GetModuleName_action_Group;
typedef cor::Group::GetTotalMembers_action_Group GetTotalMembers_action_Group;
typedef cor::Group::GetMemberList_action_Group GetMemberList_action_Group;
typedef cor::Group::GetIdp1_action_Group GetIdp1_action_Group;
typedef cor::Group::GetIdp2_action_Group GetIdp2_action_Group;
typedef cor::Group::GetIdm1_action_Group GetIdm1_action_Group;
typedef cor::Group::GetIdm2_action_Group GetIdm2_action_Group;
typedef cor::Group::GetDynamicIdp_action_Group GetDynamicIdp_action_Group;

HPX_REGISTER_ACTION_DECLARATION(Join_action_Group);
HPX_REGISTER_ACTION_DECLARATION(Leave_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetModuleName_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetTotalMembers_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetMemberList_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetIdp1_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetIdp2_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetIdm1_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetIdm2_action_Group);
HPX_REGISTER_ACTION_DECLARATION(GetDynamicIdp_action_Group);


#endif
