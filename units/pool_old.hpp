#ifndef UNITS_POOL_HPP
#define UNITS_POOL_HPP

#include "cor/cor.hpp"

#include <mutex>
#include <condition_variable>

namespace cor
{
    class Pool
    {

    public:
        Pool(std::size_t num_agents);
        ~Pool();

        void Dispatch(void (*fct)(void*), void *arg);
        void WaitForIdle();

        hpx::function<void(void *arg)> AgentFunc;
    protected:
        void PeerAgent();

    private:
        class Barrier
        {
            public:
                Barrier(std::size_t count);
                ~Barrier();
                void Wait();
                void WaitForIdle();
                void ReleaseThreads();

            private:
                std::mutex _mtx;
                std::condition_variable _qtask;
                std::condition_variable _qidle;
                std::size_t _nthreads;
                std::size_t _counter;
                bool _status;
                bool _is_idle;
        };

        void JoinAgents();

        //friend void AgentFunc(void *arg);
        

        std::size_t _num_agents;
        Barrier *_barrier;

        std::mutex _mtx;
        void (*_fct)(void*);
        void *_arg;
        bool _shut;

        idp_t _group;
        std::vector<idp_t> _agents;
    };
}

#endif
