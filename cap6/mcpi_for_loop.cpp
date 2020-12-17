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


template <typename ExPolicy, typename... Parameters>
double Func(ExPolicy&& policy, Parameters&&... params) {
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

    operon = domain->CreateLocal<cor::Operon_Client>(domain->Idp().get(),  "", pool_size).get();

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