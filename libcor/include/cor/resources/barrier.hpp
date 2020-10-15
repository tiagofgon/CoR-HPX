#ifndef COR_BARRIER_HPP
#define COR_BARRIER_HPP

#include "cor/resources/resource.hpp"
#include "cor/elements/sbarrier.hpp"

#include <hpx/hpx.hpp>

namespace cor {

// struct Barrier: public Resource, public hpx::components::component_base<Barrier>
struct Barrier: public hpx::components::abstract_migration_support< hpx::components::component_base<Barrier>, Resource >
{

using base_type = hpx::components::abstract_migration_support<
    hpx::components::component_base<Barrier>, Resource >;

typedef typename hpx::components::component_base<Barrier>::wrapping_type wrapping_type;
typedef Barrier type_holder;
typedef Resource base_type_holder;

friend class ResourceManager;

protected:
    Barrier();
    explicit Barrier(idp_t idp, idp_t clos);
    
public:
    ~Barrier();

    // Components that should be migrated using hpx::migrate<> need to
    // be Serializable and CopyConstructable. Components can be
    // MoveConstructable in which case the serialized data is moved into the
    // component's constructor.
    Barrier(Barrier&& rhs) :
        base_type(std::move(rhs)),
        sbarrier(rhs.sbarrier)
    {}

    Barrier& operator=(Barrier&& rhs)
    {
        this->Resource::operator=(std::move(static_cast<Resource&>(rhs)));
        sbarrier = rhs.sbarrier;
        return *this;
    }

    // Barrier(const Barrier&) = delete;
    // Barrier& operator=(const Barrier&) = delete;

    // Barrier(Barrier&&) noexcept;
    // Barrier& operator=(Barrier&&) noexcept;


    /* SBarrier interface */
    void Synchronize();
    
    idp_t GetIdpClos();
    idp_t GetSBarrierIdp();

    HPX_DEFINE_COMPONENT_ACTION(Barrier, Synchronize, Synchronize_action_Barrier);
    HPX_DEFINE_COMPONENT_ACTION(Barrier, GetIdpClos, GetIdpClos_action_Barrier);
    HPX_DEFINE_COMPONENT_ACTION(Barrier, GetSBarrierIdp, GetSBarrierIdp_action_Barrier);

    template <typename Archive>
    void serialize(Archive& ar, unsigned version)
    {
        ar & hpx::serialization::base_object<Resource>(*this);
        ar & sbarrier;
    }

private:
    SBarrier sbarrier;

};

}


/* SBarrier actions */
typedef cor::Barrier::Synchronize_action_Barrier Synchronize_action_Barrier;
typedef cor::Barrier::GetIdpClos_action_Barrier GetIdpClos_action_Barrier;
typedef cor::Barrier::GetSBarrierIdp_action_Barrier GetSBarrierIdp_action_Barrier;

HPX_REGISTER_ACTION_DECLARATION(Synchronize_action_Barrier);
HPX_REGISTER_ACTION_DECLARATION(GetIdpClos_action_Barrier);
HPX_REGISTER_ACTION_DECLARATION(GetSBarrierIdp_action_Barrier);


#endif
