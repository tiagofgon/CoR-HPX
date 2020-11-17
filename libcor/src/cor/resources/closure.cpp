#include "cor/resources/closure.hpp"


namespace cor {

Closure::~Closure() = default;

Closure::Closure(idp_t idp, unsigned int total_members, idp_t parent) :
    Resource{idp},
    _staticOrganizer{idp, total_members, parent}
{
    std::cout << "Criado um componente \"Closure\", com idp: " << idp << std::endl;
}


/* StaticOrganizer's interface */
void Closure::Join(idp_t idp, std::string const& name)
{
    return _staticOrganizer.Join(idp, name);
}

void Closure::Leave(idp_t idp)
{
    return _staticOrganizer.Leave(idp);
}

idp_t Closure::GetParent()
{
    return _staticOrganizer.GetParent();
}

std::size_t Closure::GetTotalMembers()
{
    return _staticOrganizer.GetTotalMembers();
}

std::size_t Closure::GetFixedTotalMembers()
{
    return _staticOrganizer.GetFixedTotalMembers();
}

std::vector<idp_t> Closure::GetMemberList()
{
    return _staticOrganizer.GetMemberList();
}

idp_t Closure::GetIdp1(idm_t idm)
{
    return _staticOrganizer.GetIdp(idm);
}

idp_t Closure::GetIdp2(std::string const& name)
{
    return _staticOrganizer.GetIdp(name);
}

idm_t Closure::GetIdm1(idp_t idp)
{
    return _staticOrganizer.GetIdm(idp);
}

idm_t Closure::GetIdm2(std::string const& name)
{
    return _staticOrganizer.GetIdm(name);
}

idp_t Closure::GetStaticOrganizerIdp()
{
    return _staticOrganizer.GetStaticOrganizerIdp();
}


}


typedef cor::Closure Closure;
typedef hpx::components::component<Closure> Closure_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(Closure_type, Closure, "Resource");


/* Action registration to interact with StaticOrganizer */
typedef cor::Closure::Join_action_Closure Join_action_Closure;
typedef cor::Closure::Leave_action_Closure Leave_action_Closure;
typedef cor::Closure::GetParent_action_Closure GetParent_action_Closure;
typedef cor::Closure::GetTotalMembers_action_Closure GetTotalMembers_action_Closure;
typedef cor::Closure::GetFixedTotalMembers_action_Closure GetFixedTotalMembers_action_Closure;
typedef cor::Closure::GetMemberList_action_Closure GetMemberList_action_Closure;
typedef cor::Closure::GetIdp1_action_Closure GetIdp1_action_Closure;
typedef cor::Closure::GetIdp2_action_Closure GetIdp2_action_Closure;
typedef cor::Closure::GetIdm1_action_Closure GetIdm1_action_Closure;
typedef cor::Closure::GetIdm2_action_Closure GetIdm2_action_Closure;
typedef cor::Closure::GetStaticOrganizerIdp_action_Closure GetStaticOrganizerIdp_action_Closure;

HPX_REGISTER_ACTION(Join_action_Closure);
HPX_REGISTER_ACTION(Leave_action_Closure);
HPX_REGISTER_ACTION(GetParent_action_Closure);
HPX_REGISTER_ACTION(GetTotalMembers_action_Closure);
HPX_REGISTER_ACTION(GetFixedTotalMembers_action_Closure);
HPX_REGISTER_ACTION(GetMemberList_action_Closure);
HPX_REGISTER_ACTION(GetIdp1_action_Closure);
HPX_REGISTER_ACTION(GetIdp2_action_Closure);
HPX_REGISTER_ACTION(GetIdm1_action_Closure);
HPX_REGISTER_ACTION(GetIdm2_action_Closure);
HPX_REGISTER_ACTION(GetStaticOrganizerIdp_action_Closure);