#include "cor/resources/rwmutex.hpp"


namespace cor {

RWMutex::~RWMutex() = default;

RWMutex::RWMutex(idp_t idp) : 
    Resource{idp}, 
    _srwmutex{idp}
{
    std::cout << "Criado um componente \"RWMutex\", com idp: " << idp << std::endl;
}


/* RWMutex interface */
void RWMutex::AcquireRead()
{
    return _srwmutex.AcquireRead();
}

void RWMutex::ReleaseRead()
{
    return _srwmutex.ReleaseRead();
}

void RWMutex::AcquireWrite()
{
    return _srwmutex.AcquireWrite();
}

void RWMutex::ReleaseWrite()
{
    return _srwmutex.ReleaseWrite();
}


}


typedef cor::RWMutex RWMutex;
typedef hpx::components::component<RWMutex> RWMutex_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(RWMutex_type, RWMutex, "Resource");


/* Action registration to interact with SRWMutex */
typedef cor::RWMutex::AcquireRead_action_RWMutex AcquireRead_action_RWMutex;
typedef cor::RWMutex::ReleaseRead_action_RWMutex ReleaseRead_action_RWMutex;
typedef cor::RWMutex::AcquireWrite_action_RWMutex AcquireWrite_action_RWMutex;
typedef cor::RWMutex::ReleaseWrite_action_RWMutex ReleaseWrite_action_RWMutex;

HPX_REGISTER_ACTION(AcquireRead_action_RWMutex);
HPX_REGISTER_ACTION(ReleaseRead_action_RWMutex);
HPX_REGISTER_ACTION(AcquireWrite_action_RWMutex);
HPX_REGISTER_ACTION(ReleaseWrite_action_RWMutex);