#include "cor/cor.hpp"
#include "units/pool.hpp"
#include "units/utils.hpp"

// System include(s):
#include <thread>
#include <vector>
 
// ROOT include(s):
#include "TROOT.h"
#include "TChain.h"
#include "TString.h"
#include "TError.h"

extern "C"
{
    void Main(int argc, char *argv[]);
}

static constexpr idm_t MASTER = 0;
 
void ReadFiles(void *)
{
    // Set up the TChain for reading all the files:
    TChain chain("CollectionTree");
    for(Int_t i = 0; i < 10; ++i) {
        chain.Add(TString::Format("/Users/brunoribeiro/Desktop/rootfiles/file%i.root", i));
    }

    // Set up reading the variables:
    Int_t intVar = 0;
    chain.SetBranchAddress("IntVar", &intVar);
    Float_t floatVar = 0;
    chain.SetBranchAddress("FloatVar", &floatVar);

    // Summary variables:
    Int_t intSum = 0;
    Float_t floatSum = 0;

    auto rank = GetRank<cor::Group>();

    // Loop over the events:
    const Long64_t entries = chain.GetEntries();
    for(Long64_t entry = 0; entry < entries; ++entry) {

        // Load the current event:
        if(chain.GetEntry(entry) <= 0) {
            Error("readFiles", "Failed to read entry %lld", entry);
            return;
        }

        // Increment the sums:
        intSum += intVar;
        floatSum += floatVar;
    }

    // Print the summed variables:
    Info("readFiles", "id = %i, intSum = %i, floatSum = %g", rank, intSum, floatSum);

    return;
}
 
void Main(int argc, char *argv[])
{
    int pool_size;

    switch(argc) {
        case 0:
            pool_size = 4;
            break;        
        case 1:
            pool_size = std::atoi(argv[0]);
            break;
        default:
            std::cerr << "ERROR: Wrong number of arguments\n";
            return;
    }

    // Enable thread correctness for ROOT:
    ROOT::EnableThreadSafety();

    //Timer t;

    auto pool = new cor::Pool(pool_size);
    auto rank = GetRank<cor::Closure>();

    std::size_t begin = 0;
    std::size_t end = 10;
    AgentRange<cor::Closure>(begin, end);
/*
    if (rank == MASTER)
        t.Start();
*/

    for (std::size_t i = begin; i < end; ++i) {
        pool->Dispatch(ReadFiles, nullptr);
        pool->WaitForIdle();
    }
/*
    if (rank == MASTER) {
        t.Stop();
        t.Report();
    }
*/
}
