#ifndef UNITS_POOL_HPP
#define UNITS_POOL_HPP

#include "cor/cor.hpp"

#include <mutex>
#include <condition_variable>
#include <hpx/hpx.hpp>

namespace cor
{
    class Pool
    {

    public:
        Pool(std::size_t num_agents);
        ~Pool();

        //void Dispatch(hpx::function<void(void*)> fct, void *arg);
        void Dispatch(void (*taskfct)(void *), void *arg);
        void WaitForIdle();

        hpx::function<void(void *arg)> AgentFunc;
        int GetRank();

    private:
        std::size_t _num_agents;
        void (*_fct)(void*);
        void *_arg;

        idp_t _group;
        std::vector<idp_t> _agents;


        std::vector<hpx::future<void>> _futures;
        std::vector<hpx::thread::id> th_ids;
        hpx::lcos::local::barrier barrier;
        int aux = 99;
    };
}

#endif
