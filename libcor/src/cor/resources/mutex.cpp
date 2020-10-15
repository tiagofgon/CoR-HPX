#include "cor/resources/mutex.hpp"

namespace cor {

// Mutex::Mutex() = default;

Mutex::Mutex(idp_t idp) : 
    ResourceNonMigrable{idp}, 
    smutex{idp}
{}

Mutex::~Mutex() = default;


void Mutex::Acquire()
{
    return smutex.Acquire();
}

void Mutex::Release()
{
    return smutex.Release();
}

}


typedef cor::Mutex Mutex;
typedef hpx::components::component<Mutex> Mutex_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(Mutex_type, Mutex, "ResourceNonMigrable");


// DynamicOranizer actions
typedef cor::Mutex::Acquire_action_Mutex Acquire_action_Mutex;
typedef cor::Mutex::Release_action_Mutex Release_action_Mutex;

HPX_REGISTER_ACTION(Acquire_action_Mutex);
HPX_REGISTER_ACTION(Release_action_Mutex);