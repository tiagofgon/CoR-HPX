#ifndef COR_RESOURCE_MIGRABLE_HPP
#define COR_RESOURCE_MIGRABLE_HPP

#include "cor/system/macros.hpp"

#include <hpx/hpx.hpp>
#include <hpx/include/actions.hpp>
#include <hpx/include/components.hpp>

namespace cor {

struct ResourceMigrable : hpx::components::abstract_base_migration_support< hpx::components::abstract_component_base<ResourceMigrable> >
{

friend class hpx::serialization::access;

protected:
    explicit ResourceMigrable(idp_t idp);

public:
    ResourceMigrable(); // needed by hpx for serialization
    virtual ~ResourceMigrable();

    // Components which should be migrated using hpx::migrate<> need to
    // be Serializable and CopyConstructable. Components can be
    // MoveConstructable in which case the serialized data is moved into the
    // component's constructor.
    ResourceMigrable(ResourceMigrable&& rhs)
      : _idp(std::move(rhs._idp))
    {
    }

    ResourceMigrable& operator=(ResourceMigrable&& rhs)
    {
        _idp = std::move(rhs._idp);
        return *this;
    }

    virtual idp_t Idp() const;
    virtual hpx::id_type GetLocalityGID();
    virtual unsigned int GetLocalityID();

    idp_t Idp_nonvirt() const { return Idp(); }
    hpx::id_type GetLocalityGID_nonvirt() { return GetLocalityGID(); }
    unsigned int GetLocalityID_nonvirt() { return GetLocalityID(); }

    HPX_DEFINE_COMPONENT_ACTION(ResourceMigrable, Idp_nonvirt, Idp_action_ResourceMigrable);
    HPX_DEFINE_COMPONENT_ACTION(ResourceMigrable, GetLocalityGID_nonvirt, GetLocalityGID_action_ResourceMigrable);
    HPX_DEFINE_COMPONENT_ACTION(ResourceMigrable, GetLocalityID_nonvirt, GetLocalityID_action_ResourceMigrable);

    template <typename Archive>
    void serialize(Archive& ar, unsigned)
    {
        ar & _idp;
    }

private:
    idp_t _idp;

};

}

typedef cor::ResourceMigrable::Idp_action_ResourceMigrable Idp_action_ResourceMigrable;
typedef cor::ResourceMigrable::GetLocalityGID_action_ResourceMigrable GetLocalityGID_action_ResourceMigrable;
typedef cor::ResourceMigrable::GetLocalityID_action_ResourceMigrable GetLocalityID_action_ResourceMigrable;

HPX_REGISTER_ACTION_DECLARATION(Idp_action_ResourceMigrable);
HPX_REGISTER_ACTION_DECLARATION(GetLocalityGID_action_ResourceMigrable);
HPX_REGISTER_ACTION_DECLARATION(GetLocalityID_action_ResourceMigrable);


#endif
