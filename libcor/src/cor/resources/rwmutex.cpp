#include "cor/services/rwmutex.hpp"

#include <hpx/hpx.hpp>

namespace cor {

RWMutex::RWMutex(idp_t idp) : 
    ResourceNonMigrable{idp}, 
    _smutex{idp}
{

}

RWMutex::~RWMutex() = default;


void RWMutex::AcquireRead()
{
    _srwmutex.AcquireRead();
}

void RWMutex::ReleaseRead()
{
    _srwmutex.ReleaseRead();
}

void RWMutex::AcquireWrite()
{
    _srwmutex.AcquireWrite();
}

void RWMutex::ReleaseWrite()
{
    _srwmutex.ReleaseWrite();
}

}


typedef cor::RWMutex RWMutex;
typedef hpx::components::component<RWMutex> RWMutex_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(RWMutex_type, RWMutex, "ResourceNonMigrable");


typedef cor::RWMutex::AcquireRead_action_RWMutex AcquireRead_action_RWMutex;
typedef cor::RWMutex::ReleaseRead_action_RWMutex ReleaseRead_action_RWMutex;
typedef cor::RWMutex::AcquireWrite_action_RWMutex AcquireWrite_action_RWMutex;
typedef cor::RWMutex::ReleaseWrite_action_RWMutex ReleaseWrite_action_RWMutex;

HPX_REGISTER_ACTION(AcquireRead_action_RWMutex);
HPX_REGISTER_ACTION(ReleaseRead_action_RWMutex);
HPX_REGISTER_ACTION(AcquireWrite_action_RWMutex);
HPX_REGISTER_ACTION(ReleaseWrite_action_RWMutex);