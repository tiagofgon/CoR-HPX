#ifndef COR_OPERON_HPP
#define COR_OPERON_HPP

#include <functional>
#include <hpx/preprocessor/cat.hpp>

#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#include "cor/resources/resource.hpp"
#include "cor/elements/executor_pool.hpp"

#include <hpx/hpx.hpp>


namespace cor {


struct Operon: public Resource, public hpx::components::component_base<Operon>
{

typedef typename hpx::components::component_base<Operon>::wrapping_type wrapping_type;
typedef Operon type_holder;
typedef Resource base_type_holder;


public:
    Operon() = delete;
    ~Operon();
    Operon(idp_t idp, std::size_t num_hpx_threads);


    void Dispatch(void (*taskfct)(void *), void *arg);
    int GetRank();
    void WaitForIdle();
    int GetNumThreads();

    std::pair<int,int> ScheduleStatic(int Beg, int End);
    std::vector<std::pair<int,int>> ScheduleStatic(int Beg, int End, int chunk);
    std::pair<int,int> ScheduleDynamic(int Beg, int End, int chunk);
    std::pair<int,int> ScheduleGuided(int Beg, int End, int chunk);

    HPX_DEFINE_COMPONENT_ACTION(Operon, GetNumThreads, GetNumThreads_action_Operon);  


private:
    ExecutorPool _executor_pool;
};

}


typedef cor::Operon::GetNumThreads_action_Operon GetNumThreads_action_Operon;

HPX_REGISTER_ACTION_DECLARATION(GetNumThreads_action_Operon);


#endif
