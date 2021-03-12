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
static std::size_t pool_size = 2;
static std::size_t in = 0, fin = 10;

COR_REGISTER_TYPE(cor::Data<Int_t>);

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

    Int_t partial = 0;
    AgentRange<cor::Closure>(in, fin);
    auto pool = new cor::Pool(pool_size);
    
    pool->Dispatch(ReadFiles, nullptr);
    pool->WaitForIdle();
    partial = std::accumulate(vec.begin(), vec.end(), partial);

    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto rank = GetRank<cor::Closure>(agent_idp);

    auto data = domain->CreateCollective<cor::Data<Int_t>>(clos_idp, domain.Idp(), "data", 0);
    auto barrier = domain->CreateCollective<cor::Barrier>(clos_idp, domain.Idp(), "barrier", clos_idp);
    
    data->AcquireWrite();
    *data->Get() += partial;
    data->ReleaseWrite();
    
    barrier->Synchronize();

    if (rank == 0) {
        data->AcquireRead();
        auto final_value = *data->Get();
        data->ReleaseRead();
        std::cout << "Final Value: " << final_value << std::endl;
    }

    delete pool;
}
