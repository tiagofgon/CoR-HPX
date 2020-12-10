#include "cor/cor.hpp"

#include "external/CpuTimer.h"
#include "external/Rand.h"

#include <vector>
#include <numeric>

std::vector<long> accepted;
long nsamples = 1000000000;

std::atomic<unsigned long> sum_static=0;


Rand R(999);

std::shared_ptr<cor::Operon_Client> operon;

extern "C"
{
    void Main(int argc);
}


struct piCalc_static {
    void operator()() {
    //std::cout << "McPi_static " << std::endl;
        unsigned long ct;
        double x, y;
        ct = 0;
        auto [beg, end] = operon->ScheduleStatic(0, nsamples);
        for (auto n = beg; n < end; ++n) {
            x = R.draw();
            y = R.draw();
            if ((x*x+y*y) <= 1.0)
                ++ct;
        }
        sum_static.fetch_add(ct);
    }
};
piCalc_static _piCalc_static;
hpx::function<void()> McPi_static(_piCalc_static);




struct piCalc_static_chunk {
    void operator()() {
        //std::cout << "McPi_static_chunk " << std::endl;
        unsigned long ct;
        double x, y;
        ct = 0;
        auto vec = operon->ScheduleStatic(0, nsamples, 500);
        for(int i=0; i<vec.size(); i++){
            auto [beg, end] = vec[i];
            for (auto n = beg; n < end; ++n) {
                x = R.draw();
                y = R.draw();
                if ((x*x+y*y) <= 1.0)
                    ++ct;
            }
        }

        sum_static.fetch_add(ct);
    }
};
piCalc_static_chunk _piCalc_static_chunk;
hpx::function<void()> McPi_static_chunk(_piCalc_static_chunk);



struct piCalc_static_dynamic {
    void operator()() {
        //std::cout << "McPi_static_chunk " << std::endl;
        unsigned long ct;
        double x, y;
        ct = 0;
        int beg, end;
        beg = 0;                    // initialize [beg, end) to global range
        end = nsamples;

        while(beg<nsamples) {
            std::pair<int, int> par = operon->ScheduleDynamic(0, nsamples, 500);
            beg=par.first;
            end=par.second;
            for(auto n = beg; n < end; ++n) {
                x = R.draw();
                y = R.draw();
                if ((x*x+y*y) <= 1.0)
                    ++ct;
            }
        }

        sum_static.fetch_add(ct);
    }
};
piCalc_static_dynamic _piCalc_static_dynamic;
hpx::function<void()> McPi_static_dynamic(_piCalc_static_dynamic);


template <typename ExPolicy, typename... Parameters>
double Func(ExPolicy&& policy, Parameters&&... params) {
    //std::cout << "McPi_static_chunk " << std::endl;
    unsigned long ct;
    ct = 0;
    int end = 1000000000;   
    auto pool_size_ = hpx::get_os_thread_count();
    std::vector<long> accepted_local(pool_size_);


    using namespace hpx::parallel;

    hpx::for_loop(execution::par(execution::task).on(std::forward<ExPolicy>(policy)).with(std::forward<Parameters>(params)...), 0, end,
    [&accepted_local](std::size_t samples) {
        unsigned long ct=0;
        auto x = R.draw();
        auto y = R.draw();
        if ((x*x+y*y) <= 1.0) {
            auto rank = hpx::get_worker_thread_num();
            accepted_local[rank]++;
        }
    });

    double result = std::accumulate(accepted_local.begin(), accepted_local.end(), result);
    double pi = 4.0 * double(result)/end;
    return pi;

}



void Main(int argc)
{
    auto domain = cor::GetDomain().get();
    CpuTimer T;
    double pi, result = 0;
    std::size_t const& pool_size = 4;

    operon = domain->CreateLocal<cor::Operon_Client>(domain->Idp(),  "", pool_size);

    /* -------------- */


    T.Start();
    auto fut1 = operon->Dispatch(McPi_static);
    fut1.get();
    pi = 4.0 * double(sum_static)/nsamples;
    std::cout << "\nValue of PI McPi_static = " << pi << std::endl;
    T.Stop();
    T.Report();


    /* -------------- */


    T.Start();
    sum_static = 0;
    auto fut2 = operon->Dispatch(McPi_static_chunk);
    fut2.get();
    pi = 4.0 * double(sum_static)/nsamples;
    std::cout << "\nValue of PI McPi_static_chunk = " << pi << std::endl;
    T.Stop();
    T.Report();


    /* -------------- */


    T.Start();
    sum_static = 0;
    auto fut3 = operon->Dispatch(McPi_static_dynamic);
    fut3.get();
    pi = 4.0 * double(sum_static)/nsamples;
    std::cout << "\nValue of PI McPi_static_dynamic = " << pi << std::endl;
    T.Stop();
    T.Report();


    /* -------------- */

    T.Start();
    // hpx::parallel::execution::parallel_executor par_exec;
    // hpx::parallel::execution::auto_chunk_size scs;

    hpx::parallel::execution::parallel_executor par_exec;
    hpx::parallel::execution::dynamic_chunk_size scs(500);

    pi = Func(par_exec, scs);

    T.Stop();

    T.Report();
    std::cout << "Value of PI = " << pi << std::endl;


}