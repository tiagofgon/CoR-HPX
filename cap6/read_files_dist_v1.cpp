#include "cor/cor.hpp"
#include "units/pool.hpp"
#include "units/utils.hpp"
#include "read_chain.hpp"

#include <vector>
#include <numeric>

extern "C"
{
    void Main(int argc, char *argv[]);
}

static std::vector<int> vec(2);
static std::size_t pool_size = 2;
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

    Int_t partial = 0;
    AgentRange<cor::Closure>(in, fin);
    auto pool = new cor::Pool(pool_size);
    
    pool->Dispatch(ReadFiles, nullptr);
    pool->WaitForIdle();
    partial = std::accumulate(vec.begin(), vec.end(), partial);

    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent<void(int,char**)>>(agent_idp);
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure>(clos_idp);
    auto clos_size = clos->GetTotalMembers();
    auto rank = clos->GetIdm(agent_idp);

    if (rank == 0) {
        Int_t final_value = partial;
        for (auto i = 1; i < clos_size; ++i) {
            auto msg = agent->Receive();
            auto res = msg.Get<Int_t>();
            final_value += res;
        }
        std::cout << "Final Value: " << final_value << std::endl;
    } else {  
        cor::Message msg;
        msg.Add<Int_t>(partial); 
        auto master_idp = clos->GetIdp(0);
        agent->Send(master_idp, msg);
    }

    delete pool;
}
