#include "cor/resources/resource_migrable.hpp"

namespace cor {

ResourceMigrable::ResourceMigrable() = default;

ResourceMigrable::ResourceMigrable(idp_t idp) : _idp{idp}
{
    //std::cout << "Criado um componente \"ResourceMigrable\", idp: " << _idp << std::endl;
}

ResourceMigrable::~ResourceMigrable() = default;

idp_t ResourceMigrable::Idp() const
{
    return _idp;
}

hpx::id_type ResourceMigrable::GetLocalityGID()
{
    return hpx::find_here();
}

unsigned int ResourceMigrable::GetLocalityID()
{
    return hpx::get_locality_id();
}

}



typedef cor::ResourceMigrable ResourceMigrable;
HPX_DEFINE_GET_COMPONENT_TYPE(ResourceMigrable);



typedef cor::ResourceMigrable::Idp_action_ResourceMigrable Idp_action_ResourceMigrable;
typedef cor::ResourceMigrable::GetLocalityGID_action_ResourceMigrable GetLocalityGID_action_ResourceMigrable;
typedef cor::ResourceMigrable::GetLocalityID_action_ResourceMigrable GetLocalityID_action_ResourceMigrable;

HPX_REGISTER_ACTION(Idp_action_ResourceMigrable);
HPX_REGISTER_ACTION(GetLocalityGID_action_ResourceMigrable);
HPX_REGISTER_ACTION(GetLocalityID_action_ResourceMigrable);
