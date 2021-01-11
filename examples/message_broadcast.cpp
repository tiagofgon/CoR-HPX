/* ---------- 
-- Tiago Gonçalves - University of Minho, 2021 --

Program that sends an integer from process with rank 0 

For execution in two separated consoles (two processes):
    $ ./corhpx apps ctx 2 0 ../examples/libmessage_broadcast.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities
    $ ./corhpx apps ctx 2 0 ../examples/libmessage_broadcast.so --hpx:hpx=localhost:1338 --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker
or using mpi:
    $ mpirun -np N ./corhpx apps ctx N 0 ../examples/libmessage_broadcast.so

---------- */

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();

    auto agent = domain->GetLocalResource<cor::Agent_Client<void(char**)>>(agent_idp);

    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto clos_size = clos->GetTotalMembers();
    auto rank = clos->GetIdm(agent_idp);
    auto parent_idp = clos->GetParent();

    // rank 0 envia a menagem para todos os outros ranks
    if(rank==0) {
        cor::Message msg;
        msg.Add<int>(12345);
        agent->Broadcast(clos_idp, msg);
    }
    // todos os outros ranks recebem a mensagem do rank 0
    else {
        auto msg = agent->Receive();
        int res = msg.Get<int>();
        std::cout << "rank " << rank << " msg received: " << res << std::endl;     
    }

    //std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << "\n";
}
