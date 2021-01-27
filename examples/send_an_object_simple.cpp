/* ---------- 
-- Tiago Gonçalves - University of Minho, 2021 --

Program that send a user object from one process to another.

For execution in two separated consoles (two processes):
    $ ./corhpx apps ctx 2 0 ../examples/libsend_an_object.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities
    $ ./corhpx apps ctx 2 0 ../examples/libsend_an_object.so --hpx:hpx=localhost:1338 --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker
or using mpi:
    $ mpirun -np 2 ./corhpx apps ctx 2 0 ../examples/libsend_an_object_simple.so

---------- */

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
        cor::Message msg;
        MsgType vec = {"Olá", "mundo", "lindo"};
        msg.Add<MsgType>(vec);

        auto other_rank = clos->GetIdp(1);
        agent->Send(other_rank, msg);
    }
    // o rank 1 recebe a mensagem
    else {
        auto msg = agent->Receive();
        MsgType vec = msg.Get<MsgType>();

        for(auto &str : vec) {
            std::cout << str << " ";
        }
        std::cout << ", received from " << msg.Sender() << std::endl;
    }

}
