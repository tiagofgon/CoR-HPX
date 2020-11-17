#ifndef COR_MUTEX_HPP
#define COR_MUTEX_HPP

#include "cor/resources/resource.hpp"
#include "cor/elements/smutex.hpp"

#include <hpx/hpx.hpp>


namespace cor {

struct Mutex: public Resource, public hpx::components::component_base<Mutex>
{

typedef hpx::components::component_base<Mutex>::wrapping_type wrapping_type;
typedef Mutex type_holder;
typedef Resource base_type_holder;

protected:
    explicit Mutex(idp_t idp);

public:
    Mutex() = delete;
    ~Mutex();

    /* Mutex's interface */
    void Acquire();
    void Release();
    
    HPX_DEFINE_COMPONENT_ACTION(Mutex, Acquire, Acquire_action_Mutex);
    HPX_DEFINE_COMPONENT_ACTION(Mutex, Release, Release_action_Mutex);

private:
    SMutex _smutex;
};

}


/* Declaration of actions to interact with SMutex */
typedef cor::Mutex::Acquire_action_Mutex Acquire_action_Mutex;
typedef cor::Mutex::Release_action_Mutex Release_action_Mutex;

HPX_REGISTER_ACTION_DECLARATION(Acquire_action_Mutex);
HPX_REGISTER_ACTION_DECLARATION(Release_action_Mutex);

#endif
