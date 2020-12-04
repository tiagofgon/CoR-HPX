#include "cor/cor.hpp"

#include "external/CpuTimer.h"
#include "external/Rand.h"

#include <vector>
#include <numeric>

std::vector<long> accepted;
long nsamples = 1000000000;

std::vector<long> accepted_static;
std::atomic<unsigned long> sum_static=0;
long nsamples_static = 500;


Rand R(999);

std::shared_ptr<cor::Operon_Client> operon;

extern "C"
{
    void Main(int argc);
}

void McPi(void *)
{
    unsigned long ct;
    double x, y;
    ct = 0;
    for (auto n = 0; n < nsamples; ++n) {
        x = R.draw();
        y = R.draw();
        if ((x*x+y*y) <= 1.0)
            ++ct;
    }
    auto rank = operon->GetRank();
    accepted[rank-1] = ct;
}

void McPi_static(void *)
{
    unsigned long ct;
    double x, y;
    ct = 0;
    auto [beg, end] = operon->ScheduleStatic(0, nsamples_static);
    for (auto n = beg; n < end; ++n) {
        x = R.draw();
        y = R.draw();
        if ((x*x+y*y) <= 1.0)
            ++ct;
    }
    sum_static.fetch_add(ct);
}

void McPi_static_chunk(void *)
{
    unsigned long ct;
    double x, y;
    ct = 0;
    auto vec = operon->ScheduleStatic(0, nsamples_static, 10);
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

void Main(int argc)
{
    auto domain = cor::GetDomain();
    CpuTimer T;
    double pi, result = 0;
    std::size_t const& pool_size = 16;


    // nsamples /= pool_size;
    // accepted.resize(pool_size, 0L);

    operon = domain->CreateLocal<cor::Operon_Client>(domain->Idp(),  "", pool_size);

    T.Start();
    // operon->Dispatch(McPi, nullptr);
    // result = std::accumulate(accepted.begin(), accepted.end(), result);
    // pi = 4.0 * (double) (result/(pool_size*nsamples));
    // std::cout << "Value of PI = " << pi << std::endl;

    // operon->Dispatch(McPi_static, nullptr);
    // pi = 4.0 * double(sum_static)/nsamples_static;
    // std::cout << "Value of PI Static = " << pi << std::endl;

    operon->Dispatch(McPi_static_chunk, nullptr);
    pi = 4.0 * double(sum_static)/nsamples_static;
    std::cout << "Value of PI Static = " << pi << std::endl;

    T.Stop();

    T.Report();
    

}