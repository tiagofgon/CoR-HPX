#include "cor/resources/resource_non_migrable.hpp"

namespace cor {

ResourceNonMigrable::ResourceNonMigrable() = default;

ResourceNonMigrable::ResourceNonMigrable(idp_t idp) : _idp{idp}
{
    std::cout << "Criado um componente \"ResourceNonMigrable\", idp: " << _idp << std::endl;
}

ResourceNonMigrable::~ResourceNonMigrable() = default;

// ResourceNonMigrable::ResourceNonMigrable(ResourceNonMigrable&&) noexcept = default;

// ResourceNonMigrable& ResourceNonMigrable::operator=(ResourceNonMigrable&&) noexcept = default;

idp_t ResourceNonMigrable::Idp() const
{
    return _idp;
}

hpx::id_type ResourceNonMigrable::GetLocalityGID()
{
    return hpx::find_here();
}

unsigned int ResourceNonMigrable::GetLocalityID()
{
    return hpx::get_locality_id();
}

}



typedef cor::ResourceNonMigrable ResourceNonMigrable;
// typedef hpx::components::component<cor::ResourceNonMigrable> ResourceNonMigrable_type;

// HPX_REGISTER_COMPONENT(ResourceNonMigrable_type, ResourceNonMigrable);

HPX_DEFINE_GET_COMPONENT_TYPE(ResourceNonMigrable);



typedef cor::ResourceNonMigrable::Idp_action_ResourceNonMigrable Idp_action_ResourceNonMigrable;
typedef cor::ResourceNonMigrable::GetLocalityGID_action_ResourceNonMigrable GetLocalityGID_action_ResourceNonMigrable;
typedef cor::ResourceNonMigrable::GetLocalityID_action_ResourceNonMigrable GetLocalityID_action_ResourceNonMigrable;

HPX_REGISTER_ACTION(Idp_action_ResourceNonMigrable);
HPX_REGISTER_ACTION(GetLocalityGID_action_ResourceNonMigrable);
HPX_REGISTER_ACTION(GetLocalityID_action_ResourceNonMigrable);
