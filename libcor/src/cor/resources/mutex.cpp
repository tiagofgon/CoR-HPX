#include "cor/resources/mutex.hpp"


namespace cor {

Mutex::~Mutex() = default;

Mutex::Mutex(idp_t idp) : 
    Resource{idp}, 
    _smutex{idp}
{
    std::cout << "Criado um componente \"Mutex\", com idp: " << idp << std::endl;
}


/* Mutex interface */
void Mutex::Acquire()
{
    return _smutex.Acquire();
}

void Mutex::Release()
{
    return _smutex.Release();
}


}


typedef cor::Mutex Mutex;
typedef hpx::components::component<Mutex> Mutex_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(Mutex_type, Mutex, "Resource");


/* Action registration to interact with SMutex */
typedef cor::Mutex::Acquire_action_Mutex Acquire_action_Mutex;
typedef cor::Mutex::Release_action_Mutex Release_action_Mutex;

HPX_REGISTER_ACTION(Acquire_action_Mutex);
HPX_REGISTER_ACTION(Release_action_Mutex);