#ifndef COR_BARRIER_HPP
#define COR_BARRIER_HPP

#include "cor/resources/resource.hpp"
#include "cor/elements/sbarrier.hpp"

#include <hpx/hpx.hpp>


namespace cor {

struct Barrier: public Resource, public hpx::components::component_base<Barrier>
{

typedef typename hpx::components::component_base<Barrier>::wrapping_type wrapping_type;
typedef Barrier type_holder;
typedef Resource base_type_holder;

protected:
    explicit Barrier(idp_t idp, idp_t clos);
    
public:
    Barrier() = delete;
    ~Barrier();

    /* SBarrier's interface */
    void Synchronize();
    
    idp_t GetIdpClos();
    idp_t GetSBarrierIdp();

    HPX_DEFINE_COMPONENT_ACTION(Barrier, Synchronize, Synchronize_action_Barrier);
    HPX_DEFINE_COMPONENT_ACTION(Barrier, GetIdpClos, GetIdpClos_action_Barrier);
    HPX_DEFINE_COMPONENT_ACTION(Barrier, GetSBarrierIdp, GetSBarrierIdp_action_Barrier);


private:
    SBarrier _sbarrier;

};

}


/* Declaration of actions to interact with SBarrier */
typedef cor::Barrier::Synchronize_action_Barrier Synchronize_action_Barrier;
typedef cor::Barrier::GetIdpClos_action_Barrier GetIdpClos_action_Barrier;
typedef cor::Barrier::GetSBarrierIdp_action_Barrier GetSBarrierIdp_action_Barrier;

HPX_REGISTER_ACTION_DECLARATION(Synchronize_action_Barrier);
HPX_REGISTER_ACTION_DECLARATION(GetIdpClos_action_Barrier);
HPX_REGISTER_ACTION_DECLARATION(GetSBarrierIdp_action_Barrier);


#endif
