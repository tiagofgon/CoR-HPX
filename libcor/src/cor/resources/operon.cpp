#include "cor/resources/operon.hpp"

namespace cor {

Operon::~Operon() = default;

Operon::Operon(idp_t idp, std::size_t num_hpx_threads) :
    Resource{idp},
    _executor_pool{idp, num_hpx_threads}
{
    std::cout << "Criado um componente \"Operon\", com idp: " << idp << ", com threads: " << num_hpx_threads << std::endl;
}


/* DynamicOrganizer's interface */
void Operon::Dispatch(void (*taskfct)(void *), void *arg)
{
    return _executor_pool.Dispatch(taskfct, arg);
}

int Operon::GetRank()
{
    return _executor_pool.GetRank();
}

void Operon::WaitForIdle()
{
    return _executor_pool.WaitForIdle();
}

int Operon::GetNumThreads() {
    return _executor_pool.GetNumThreads();
}

std::pair<int,int> Operon::ScheduleStatic(int Beg, int End)
{
    return _executor_pool.ScheduleStatic(Beg, End);
}

std::pair<int,int> Operon::ScheduleDynamic(int Beg, int End, int chunk)
{
    return _executor_pool.ScheduleDynamic(Beg, End, chunk);
}

std::pair<int,int> Operon::ScheduleGuided(int Beg, int End, int chunk)
{
    return _executor_pool.ScheduleGuided(Beg, End, chunk);
}



}


typedef cor::Operon Operon;
typedef hpx::components::component<Operon> Operon_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(Operon_type, Operon, "Resource");


/* Action registration to interact with DynamicOranizer */
typedef cor::Operon::GetNumThreads_action_Operon GetNumThreads_action_Operon;

HPX_REGISTER_ACTION(GetNumThreads_action_Operon);
