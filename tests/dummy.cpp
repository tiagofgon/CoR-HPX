// System include(s):
#include "cor/cor.hpp"
#include "units/pool.hpp"

//#include <thread>
#include <vector>
#include <numeric>

// POOL include(s)
//#include "include/CpuTimer.h"
//#include "include/Rand.h"
//#include "include/PoolAdditional.h"
#include "units/utils.hpp"

// ROOT include(s):
#include "TROOT.h"
#include "TChain.h"
#include "TString.h"
#include "TError.h"

extern "C"
{
    void Main(int argc, char *argv[]);
}

COR_REGISTER_TYPE(cor::Data<std::vector<int>>);

static std::vector<std::size_t> vec(2);
static constexpr idm_t MASTER = 0;

void readFiles(void *id)
{
    auto fid = static_cast<std::size_t*>(id);

    // Set up the TChain for reading all the files:
    TChain chain("CollectionTree");
    chain.Add(TString::Format("/Users/brunoribeiro/Desktop/rootfiles/file%lu.root", *fid));

    // Set up reading the variables:
    Int_t intVar = 0;
    chain.SetBranchAddress("IntVar", &intVar);
    Float_t floatVar = 0;
    chain.SetBranchAddress("FloatVar", &floatVar);

    // Summary variables:
    Int_t intSum = 0;
    Float_t floatSum = 0;

    // Loop over the events:
    auto entries = chain.GetEntries();

    std::size_t beg, end; 

    beg = 0;
    end = entries;

    AgentRange<cor::Group>(beg, end);
    auto rank = GetRank<cor::Group>();

    for (auto entry = beg; entry < end; ++entry) {
        
        if (chain.GetEntry(entry) <= 0) {
            Error("readFiles", "Failed to read entry %lu", entry);
            return;
        }

        // Increment the sums:
        intSum += intVar;
        floatSum += floatVar;
    }

    // Print the summed variables:
    auto nF = TString::Format("file%lu.root", *fid);

    Info("", "file = %s, Entries = %lu, id = %i, intSum = %i, floatSum = %g", nF.Data(), end-beg, rank, intSum, floatSum);
    vec[rank] = intSum;
}

void Main(int argc, char *argv[])
{
    size_t size = 2;
    size_t fvalue = 0;

    if (argc >= 1) {
        size = std::atoi(argv[0]);
        vec.resize(size);
    }

    // Enable thread correctness for ROOT:
    ROOT::EnableThreadSafety();
    
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto comm_idp = domain->GetPredecessorIdp(agent_idp);

    auto pool = new cor::Pool(size);

    std::size_t res, beg, end;
    res = 0; beg = 0; end = 10;

    AgentRange<cor::Closure>(beg, end);

    for(auto i = beg; i < end ; ++i) {
        pool->Dispatch(readFiles, &i);
        pool->WaitForIdle();
        res = std::accumulate(vec.begin(), vec.end(), res);
    }

    auto rank = GetRank<cor::Closure>(agent_idp);
    auto comm_size = GetSize<cor::Closure>(comm_idp);

    auto data = domain->CreateCollective<cor::Data<std::vector<int>>>(comm_idp, domain->Idp(), "data", comm_size);
    auto barrier = domain->CreateCollective<cor::Barrier>(comm_idp, domain->Idp(), "barrier", comm_idp);

    {
        data->AcquireWrite();
        auto aux = data->Get();
        (*aux)[rank] = res;
        data->ReleaseWrite();
    }

    barrier->Synchronize();

    if (rank == MASTER) {

        data->AcquireRead();
        auto aux = data->Get();
        fvalue = std::accumulate((*aux).begin(), (*aux).end(), 0);
        data->ReleaseRead();

        std::cout << "Final Value: " << fvalue << std::endl;
    }

}
