#include "cor/resources/resource.hpp"

namespace cor {

Resource::Resource() = default;

Resource::Resource(idp_t idp) : _idp{idp}
{
    //std::cout << "Criado um componente \"Resource\", idp: " << _idp << std::endl;
}

Resource::~Resource() = default;

idp_t Resource::Idp() const
{
    return _idp;
}

hpx::id_type Resource::GetLocalityGID()
{
    return hpx::find_here();
}

unsigned int Resource::GetLocalityID()
{
    return hpx::get_locality_id();
}

}



typedef cor::Resource Resource;
HPX_DEFINE_GET_COMPONENT_TYPE(Resource);



typedef cor::Resource::Idp_action_Resource Idp_action_Resource;
typedef cor::Resource::GetLocalityGID_action_Resource GetLocalityGID_action_Resource;
typedef cor::Resource::GetLocalityID_action_Resource GetLocalityID_action_Resource;

HPX_REGISTER_ACTION(Idp_action_Resource);
HPX_REGISTER_ACTION(GetLocalityGID_action_Resource);
HPX_REGISTER_ACTION(GetLocalityID_action_Resource);
