#include "cor/services/mutex_service.hpp"

#include <hpx/hpx.hpp>

namespace cor {

MutexService::MutexService()
{
    std::cout << "Criado um componente MutexService" << std::endl;
}

MutexService::~MutexService() = default;


void MutexService::Acquire()
{
    std::cout << "lock()1" << std::endl;
    _mtx.lock();
    std::cout << "lock()2" << std::endl;
}

void MutexService::Release()
{
    std::cout << "unlock()1" << std::endl;
    _mtx.unlock();
    std::cout << "unlock()2" << std::endl;
}

}


typedef cor::MutexService MutexService;
typedef hpx::components::component<MutexService> MutexService_type;

HPX_REGISTER_COMPONENT(MutexService_type, MutexService);


typedef cor::MutexService::Acquire_action_MutexService Acquire_action_MutexService;
typedef cor::MutexService::Release_action_MutexService Release_action_MutexService;

HPX_REGISTER_ACTION(Acquire_action_MutexService);
HPX_REGISTER_ACTION(Release_action_MutexService);