#ifndef COR_RESOURCE_HPP
#define COR_RESOURCE_HPP

#include "cor/system/macros.hpp"

#include <hpx/hpx.hpp>
#include <hpx/include/actions.hpp>
#include <hpx/include/components.hpp>

namespace cor {

// struct Resource : hpx::components::abstract_component_base<Resource>
struct Resource : hpx::components::abstract_component_base<Resource>
{

protected:
    explicit Resource(idp_t idp);

public:
    Resource(); // needed by hpx for serialization
    virtual ~Resource();
    
    virtual idp_t Idp() const;
    virtual hpx::id_type GetLocalityGID();
    virtual unsigned int GetLocalityID();

    idp_t Idp_nonvirt() const { return Idp(); }
    hpx::id_type GetLocalityGID_nonvirt() { return GetLocalityGID(); }
    unsigned int GetLocalityID_nonvirt() { return GetLocalityID(); }

    HPX_DEFINE_COMPONENT_ACTION(Resource, Idp_nonvirt, Idp_action_Resource);
    HPX_DEFINE_COMPONENT_ACTION(Resource, GetLocalityGID_nonvirt, GetLocalityGID_action_Resource);
    HPX_DEFINE_COMPONENT_ACTION(Resource, GetLocalityID_nonvirt, GetLocalityID_action_Resource);



private:
    idp_t _idp;

};

}

typedef cor::Resource::Idp_action_Resource Idp_action_Resource;
typedef cor::Resource::GetLocalityGID_action_Resource GetLocalityGID_action_Resource;
typedef cor::Resource::GetLocalityID_action_Resource GetLocalityID_action_Resource;

HPX_REGISTER_ACTION_DECLARATION(Idp_action_Resource);
HPX_REGISTER_ACTION_DECLARATION(GetLocalityGID_action_Resource);
HPX_REGISTER_ACTION_DECLARATION(GetLocalityID_action_Resource);


#endif
