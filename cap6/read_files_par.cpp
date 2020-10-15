#include "cor/cor.hpp"
#include "units/pool.hpp"
#include "units/utils.hpp"
#include "read_chain.hpp"
#include "external/CpuTimer.h"

#include <vector>
#include <numeric>

extern "C"
{
    void Main(int argc, char *argv[]);
}

static std::vector<int> vec(2);
static size_t pool_size = 2;
static std::size_t in = 0, fin = 10;

void ReadFiles(void *)
{
    ReadChain rc("CollectionTree", "/Users/brunoribeiro/Desktop/rootfiles");
    rc.SetFileRange(in, fin);                                   
    rc.Initialize();
    rc.ProcessChain();
    auto rank = GetRank<cor::Group>();
    vec[rank] = rc.GetIntSum();
}

void Main(int argc, char *argv[])
{
    if (argc >= 1) { pool_size = std::atoi(argv[0]); vec.resize(pool_size); }
    if (argc >= 2) in = std::atoi(argv[1]);
    if (argc >= 3) fin = std::atoi(argv[2]);

    ROOT::EnableThreadSafety();
    CpuTimer T;
    Int_t result = 0;
    auto pool = new cor::Pool(pool_size);

    T.Start();
    pool->Dispatch(ReadFiles, nullptr);
    pool->WaitForIdle();
    result = std::accumulate(vec.begin(), vec.end(), result);
    T.Stop();

    T.Report();
    std::cout << "Final Value: " << result << std::endl;
}
