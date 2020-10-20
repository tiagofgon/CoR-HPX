#ifndef COR_MUTEX_NON_MIGRABLE_HPP
#define COR_MUTEX_NON_MIGRABLE_HPP

#include "cor/resources/resource_non_migrable.hpp"
#include "cor/elements/smutex.hpp"

namespace cor {

struct Mutex: public ResourceNonMigrable, public hpx::components::component_base<Mutex>
{

typedef hpx::components::component_base<Mutex>::wrapping_type wrapping_type;
typedef Mutex type_holder;
typedef ResourceNonMigrable base_type_holder;

protected:
explicit Mutex(idp_t idp);

public:
    Mutex();
    ~Mutex();

    void Acquire();
    void Release();
    
    HPX_DEFINE_COMPONENT_ACTION(Mutex, Acquire, Acquire_action_Mutex);
    HPX_DEFINE_COMPONENT_ACTION(Mutex, Release, Release_action_Mutex);

private:
    SMutex _smutex;

};

}


typedef cor::Mutex::Acquire_action_Mutex Acquire_action_Mutex;
typedef cor::Mutex::Release_action_Mutex Release_action_Mutex;

HPX_REGISTER_ACTION_DECLARATION(Acquire_action_Mutex);
HPX_REGISTER_ACTION_DECLARATION(Release_action_Mutex);

#endif
