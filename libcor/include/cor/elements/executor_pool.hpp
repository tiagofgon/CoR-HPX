#ifndef EXECUTOR_POOL_HPP
#define EXECUTOR_POOL_HPP

//#include "cor/cor.hpp"

#include <mutex>
#include <condition_variable>
#include <hpx/hpx.hpp>

namespace cor
{
    class ExecutorPool
    {

    public:
        ExecutorPool(idp_t idp, std::size_t num_hpx_threads);
        ~ExecutorPool();

        int GetRank();
        int GetNumThreads();

        std::pair<int,int> ScheduleStatic(int Beg, int End);
        std::vector<std::pair<int,int>> ScheduleStatic(int Beg, int End, int chunk);
        std::pair<int,int> ScheduleDynamic(int Beg, int End, int chunk);
        std::pair<int,int> ScheduleGuided(int Beg, int End, int chunk);

        template < typename ... Args >
        void Dispatch(hpx::function<void(Args...)> func, Args ... args);

        void Dispatch(hpx::function<void()> func);

        template < typename Func, typename ... Args >
        void Dispatch(Func&& func, Args&&... args);
        
        template < typename Func>
        void Dispatch(Func&& func);

    private:
        idp_t _idp;
        std::size_t _num_hpx_threads;

        std::vector<hpx::future<void>> _futures;
        //std::vector<hpx::thread::id> th_ids;
        std::map<hpx::thread::id, int> th_ids; // map that hold the ranks

        // index for sheduling
        hpx::mutex _mtx;
        std::atomic<int> _index;
        std::atomic<int> th_ids_index; // for rank distribution
        hpx::lcos::local::spinlock _mtx2;
    };
}

#include "cor/elements/executor_pool.tpp"

#endif
