// Tiago Gonçalves & António Pina - University of Minho - LIP, 2021
//
// Program that sends an integer from pod 0 to others from same closure
//
// Console 1: ./corhpx apps ctx 2 1 0 ../examples/libmessage_broadcast.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities
// Console 2: ./corhpx apps ctx 2 1 0 ../examples/libmessage_broadcast.so --hpx:hpx=localhost:1338 --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker
// 
// With MPI in one console only: mpirun -np 2 ./corhpx apps ctx 2 1 0 ../examples/libmessage_broadcast.so
//
// With virtual pods: ./corhpx apps ctx 1 2 0 ../examples/libmessage_broadcast.so
//

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
    auto clos_size = clos->GetTotalMembers();
    auto rank = clos->GetIdm(agent_idp);
    auto parent_idp = clos->GetParent();

    // rank 0 envia a menagem para todos os outros ranks
    if(rank==0) {
        cor::Message msg;
        MsgType vec = {"Olá", "mundo", "lindo"};
        msg.Add<MsgType>(vec);
        agent->Broadcast(clos_idp, msg);
    }
    // todos os outros ranks recebem a mensagem do rank 0
    else {
        auto msg = agent->Receive();
        MsgType vec = msg.Get<MsgType>();
        std::cout << "rank " << rank << " msg received: ";
        for(auto &str : vec) {
            std::cout << str << " ";
        } 
        std::cout << std::endl;
    }

    //std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << "\n";
}
