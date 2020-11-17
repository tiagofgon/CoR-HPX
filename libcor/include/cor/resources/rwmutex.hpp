#ifndef COR_RWMUTEX_HPP
#define COR_RWMUTEX_HPP

#include "cor/resources/resource.hpp"
#include "cor/elements/srwmutex.hpp"

#include <hpx/hpx.hpp>


namespace cor {

struct RWMutex: public Resource, public hpx::components::component_base<RWMutex>
{

typedef hpx::components::component_base<RWMutex>::wrapping_type wrapping_type;
typedef RWMutex type_holder;
typedef Resource base_type_holder;

protected:
    explicit RWMutex(idp_t idp);

public:
    RWMutex() = delete;
    ~RWMutex();

    /* RWMutex's interface */
    void AcquireRead();
    void ReleaseRead();
    void AcquireWrite();
    void ReleaseWrite();
    
    HPX_DEFINE_COMPONENT_ACTION(RWMutex, AcquireRead, AcquireRead_action_RWMutex);
    HPX_DEFINE_COMPONENT_ACTION(RWMutex, ReleaseRead, ReleaseRead_action_RWMutex);
    HPX_DEFINE_COMPONENT_ACTION(RWMutex, AcquireWrite, AcquireWrite_action_RWMutex);
    HPX_DEFINE_COMPONENT_ACTION(RWMutex, ReleaseWrite, ReleaseWrite_action_RWMutex);

private:
    SRWMutex _srwmutex;
};

}


/* Declaration of actions to interact with SRWMutex */
typedef cor::RWMutex::AcquireRead_action_RWMutex AcquireRead_action_RWMutex;
typedef cor::RWMutex::ReleaseRead_action_RWMutex ReleaseRead_action_RWMutex;
typedef cor::RWMutex::AcquireWrite_action_RWMutex AcquireWrite_action_RWMutex;
typedef cor::RWMutex::ReleaseWrite_action_RWMutex ReleaseWrite_action_RWMutex;

HPX_REGISTER_ACTION_DECLARATION(AcquireRead_action_RWMutex);
HPX_REGISTER_ACTION_DECLARATION(ReleaseRead_action_RWMutex);
HPX_REGISTER_ACTION_DECLARATION(AcquireWrite_action_RWMutex);
HPX_REGISTER_ACTION_DECLARATION(ReleaseWrite_action_RWMutex);

#endif
