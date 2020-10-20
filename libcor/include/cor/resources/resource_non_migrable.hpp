#ifndef COR_RESOURCE_NON_MIGRABLE_HPP
#define COR_RESOURCE_NON_MIGRABLE_HPP

#include "cor/system/macros.hpp"

#include <hpx/hpx.hpp>
#include <hpx/include/actions.hpp>
#include <hpx/include/components.hpp>

namespace cor {

// struct ResourceNonMigrable : hpx::components::abstract_component_base<ResourceNonMigrable>
struct ResourceNonMigrable : hpx::components::abstract_component_base<ResourceNonMigrable>
{

protected:
    explicit ResourceNonMigrable(idp_t idp);

public:
    ResourceNonMigrable(); // needed by hpx for serialization
    virtual ~ResourceNonMigrable();
    
    virtual idp_t Idp() const;
    virtual hpx::id_type GetLocalityGID();
    virtual unsigned int GetLocalityID();

    idp_t Idp_nonvirt() const { return Idp(); }
    hpx::id_type GetLocalityGID_nonvirt() { return GetLocalityGID(); }
    unsigned int GetLocalityID_nonvirt() { return GetLocalityID(); }

    HPX_DEFINE_COMPONENT_ACTION(ResourceNonMigrable, Idp_nonvirt, Idp_action_ResourceNonMigrable);
    HPX_DEFINE_COMPONENT_ACTION(ResourceNonMigrable, GetLocalityGID_nonvirt, GetLocalityGID_action_ResourceNonMigrable);
    HPX_DEFINE_COMPONENT_ACTION(ResourceNonMigrable, GetLocalityID_nonvirt, GetLocalityID_action_ResourceNonMigrable);



private:
    idp_t _idp;

};

}

typedef cor::ResourceNonMigrable::Idp_action_ResourceNonMigrable Idp_action_ResourceNonMigrable;
typedef cor::ResourceNonMigrable::GetLocalityGID_action_ResourceNonMigrable GetLocalityGID_action_ResourceNonMigrable;
typedef cor::ResourceNonMigrable::GetLocalityID_action_ResourceNonMigrable GetLocalityID_action_ResourceNonMigrable;

HPX_REGISTER_ACTION_DECLARATION(Idp_action_ResourceNonMigrable);
HPX_REGISTER_ACTION_DECLARATION(GetLocalityGID_action_ResourceNonMigrable);
HPX_REGISTER_ACTION_DECLARATION(GetLocalityID_action_ResourceNonMigrable);


#endif
