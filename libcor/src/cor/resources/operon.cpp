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
int Operon::GetRank()
{
    return _executor_pool.GetRank();
}

int Operon::GetNumThreads() {
    return _executor_pool.GetNumThreads();
}

std::pair<int,int> Operon::ScheduleStatic(int Beg, int End)
{
    return _executor_pool.ScheduleStatic(Beg, End);
}

std::vector<std::pair<int,int>> Operon::ScheduleStatic(int Beg, int End, int chunk)
{
    return _executor_pool.ScheduleStatic(Beg, End, chunk);
}

std::pair<int,int> Operon::ScheduleDynamic(int Beg, int End, int chunk)
{
    return _executor_pool.ScheduleDynamic(Beg, End, chunk);
}

std::pair<int,int> Operon::ScheduleGuided(int Beg, int End, int chunk)
{
    return _executor_pool.ScheduleGuided(Beg, End, chunk);
}

void Operon::Dispatch_void(hpx::function<void()> func)
{
    return _executor_pool.Dispatch(func);
}



}


typedef cor::Operon Operon;
typedef hpx::components::component<Operon> Operon_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(Operon_type, Operon, "Resource");


/* Action registration to interact with DynamicOranizer */
typedef cor::Operon::GetNumThreads_action_Operon GetNumThreads_action_Operon;
typedef cor::Operon::Dispatch_void_action_Operon Dispatch_void_action_Operon;

HPX_REGISTER_ACTION(GetNumThreads_action_Operon);
HPX_REGISTER_ACTION(Dispatch_void_action_Operon);
