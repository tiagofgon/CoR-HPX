#include "cor/services/mutexRW_service.hpp"

#include <hpx/hpx.hpp>

namespace cor {

MutexRWService::MutexRWService()
{
    //std::cout << "Criado um componente MutexRWService" << std::endl;
}

MutexRWService::~MutexRWService() = default;


void MutexRWService::AcquireRead()
{
    // std::cout << "lock()1" << std::endl;
    _mtx.lock_shared();
    // std::cout << "lock()2" << std::endl;
}

void MutexRWService::ReleaseRead()
{
    // std::cout << "unlock()1" << std::endl;
    _mtx.unlock_shared();
    // std::cout << "unlock()2" << std::endl;
}

void MutexRWService::AcquireWrite()
{
    // std::cout << "lock()1" << std::endl;
    _mtx.lock();
    // std::cout << "lock()2" << std::endl;
}

void MutexRWService::ReleaseWrite()
{
    // std::cout << "unlock()1" << std::endl;
    _mtx.unlock();
    // std::cout << "unlock()2" << std::endl;
}

}


typedef cor::MutexRWService MutexRWService;
typedef hpx::components::component<MutexRWService> MutexRWService_type;

HPX_REGISTER_COMPONENT(MutexRWService_type, MutexRWService);


typedef cor::MutexRWService::AcquireRead_action_MutexRWService AcquireRead_action_MutexRWService;
typedef cor::MutexRWService::ReleaseRead_action_MutexRWService ReleaseRead_action_MutexRWService;
typedef cor::MutexRWService::AcquireWrite_action_MutexRWService AcquireWrite_action_MutexRWService;
typedef cor::MutexRWService::ReleaseWrite_action_MutexRWService ReleaseWrite_action_MutexRWService;

HPX_REGISTER_ACTION(AcquireRead_action_MutexRWService);
HPX_REGISTER_ACTION(ReleaseRead_action_MutexRWService);
HPX_REGISTER_ACTION(AcquireWrite_action_MutexRWService);
HPX_REGISTER_ACTION(ReleaseWrite_action_MutexRWService);