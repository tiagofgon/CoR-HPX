#ifndef COR_MAUTEXRW_SERVICE_HPP
#define COR_MAUTEXRW_SERVICE_HPP

#include <hpx/hpx.hpp>

namespace cor {

struct MutexRWService: public hpx::components::component_base<MutexRWService>
{

public:
    MutexRWService();
    ~MutexRWService();

    void AcquireRead();
    void ReleaseRead();

    void AcquireWrite();
    void ReleaseWrite();
    
    HPX_DEFINE_COMPONENT_ACTION(MutexRWService, AcquireRead, AcquireRead_action_MutexRWService);
    HPX_DEFINE_COMPONENT_ACTION(MutexRWService, ReleaseRead, ReleaseRead_action_MutexRWService);
    HPX_DEFINE_COMPONENT_ACTION(MutexRWService, AcquireWrite, AcquireWrite_action_MutexRWService);
    HPX_DEFINE_COMPONENT_ACTION(MutexRWService, ReleaseWrite, ReleaseWrite_action_MutexRWService);

private:
    hpx::lcos::local::shared_mutex _mtx;

};

}


typedef cor::MutexRWService::AcquireRead_action_MutexRWService AcquireRead_action_MutexRWService;
typedef cor::MutexRWService::ReleaseRead_action_MutexRWService ReleaseRead_action_MutexRWService;
typedef cor::MutexRWService::AcquireWrite_action_MutexRWService AcquireWrite_action_MutexRWService;
typedef cor::MutexRWService::ReleaseWrite_action_MutexRWService ReleaseWrite_action_MutexRWService;

HPX_REGISTER_ACTION_DECLARATION(AcquireRead_action_MutexRWService);
HPX_REGISTER_ACTION_DECLARATION(ReleaseRead_action_MutexRWService);
HPX_REGISTER_ACTION_DECLARATION(AcquireWrite_action_MutexRWService);
HPX_REGISTER_ACTION_DECLARATION(ReleaseWrite_action_MutexRWService);

#endif
