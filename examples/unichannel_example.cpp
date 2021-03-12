/* ---------- 
-- Tiago Gonçalves - University of Minho, 2021 --

---------- */

/*
    Tiago Gonçalves & António Pina, UM - LIP, 2021

    To run: 
        ./corhpx apps ctx 1 0 ../examples/libunichannel_example.so
*/

/*
    Tiago Gonçalves & António Pina, UM - LIP, 2021

    Program to be run in parallel with two processes, only.
    Sends a user object from one process to another.

    To run: 
        Console 1: ./corhpx apps ctx 2 0 ../examples/libunichannel_example.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities
        Console 2: ./corhpx apps ctx 2 0 ../examples/libunichannel_example.so --hpx:hpx=localhost:1338 --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker
        With MPI: mpirun -np 2 ./corhpx apps ctx 2 0 ../examples/libunichannel_example.so
*/


#include "cor/cor.hpp"


extern "C"
{
    void Main(int argc, char *argv[]);
}

typedef std::vector<std::string> MsgType;

void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(char**)>>(agent_idp);

    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto rank = clos->GetIdm(agent_idp);

    if(clos->GetTotalMembers() != 2) return;

    // rank 0 envia a menagem para o rank 1
    if(rank==0) {
        auto channel = domain->CreateLocal<cor::UniChannel_Client<std::vector<double>>>(domain->Idp(), "unichannel", "w");
        std::vector<double> vec = {1, 2, 3};
        channel->Set(std::move(vec), 0);
    }
    // o rank 1 recebe a mensagem
    else {

        std::vector<idp_t> remote_domains = domain->GetRemoteDomains();
        idp_t remote_domain_idp = remote_domains[0];
        auto remote_domain_client = domain->CreateReference<cor::Domain_Client>(remote_domain_idp, domain->Idp(), "");
        auto channel_idp = remote_domain_client->GetIdp("unichannel");
        auto channel = domain->CreateReference<cor::UniChannel_Client<std::vector<double>>>(channel_idp, domain->Idp(),  "");


        auto vec = channel->Get(0).get();
        for (auto v : vec) {
            std::cout << v << std::endl;
        }
    }
}
