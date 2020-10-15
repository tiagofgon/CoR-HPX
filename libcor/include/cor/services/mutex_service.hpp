#ifndef COR_MAUTEX_SERVICE_HPP
#define COR_MAUTEX_SERVICE_HPP

#include <hpx/hpx.hpp>

namespace cor {

struct MutexService: public hpx::components::component_base<MutexService>
{

public:
    MutexService();
    ~MutexService();

    void Acquire();
    void Release();
    
    HPX_DEFINE_COMPONENT_ACTION(MutexService, Acquire, Acquire_action_MutexService);
    HPX_DEFINE_COMPONENT_ACTION(MutexService, Release, Release_action_MutexService);

private:
    hpx::lcos::local::spinlock _mtx;

};

}


typedef cor::MutexService::Acquire_action_MutexService Acquire_action_MutexService;
typedef cor::MutexService::Release_action_MutexService Release_action_MutexService;

HPX_REGISTER_ACTION_DECLARATION(Acquire_action_MutexService);
HPX_REGISTER_ACTION_DECLARATION(Release_action_MutexService);

#endif
