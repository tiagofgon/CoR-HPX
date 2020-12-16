#include "cor/cor.hpp"
#include "units/pool.hpp"
#include "units/utils.hpp"
#include "external/CpuTimer.h"
#include "external/Rand.h"

#include <vector>
#include <numeric>

std::vector<long> accepted;
long nsamples = 1000000000;
Rand R(999);

extern "C"
{
    void Main(int argc, char *argv[]);
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
    auto rank = GetRank<cor::Group>();
    accepted[rank] = ct;
}

void Main(int argc, char *argv[])
{
    CpuTimer T;
    double pi, result = 0;
    std::size_t pool_size = 4;

    if (argc >= 1) pool_size = std::atoi(argv[0]);
    if (argc == 2) nsamples = atoi(argv[1]);

    nsamples /= pool_size;
    accepted.resize(pool_size, 0L);
    auto pool = new cor::Pool(pool_size);

    T.Start();
    pool->Dispatch(McPi, nullptr);
    pool->WaitForIdle();
    result = std::accumulate(accepted.begin(), accepted.end(), result);
    pi = 4.0 * (double) (result/(pool_size*nsamples));
    T.Stop();

    T.Report();
    std::cout << "Value of PI = " << pi << std::endl;

    delete pool;
}