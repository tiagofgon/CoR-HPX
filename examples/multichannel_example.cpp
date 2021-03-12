/*
    Tiago Gonçalves & António Pina, UM - LIP, 2021

    Program to be run in parallel with two processes, only.

    To run: 
        Console 1: ./corhpx apps ctx 2 0 ../examples/libmultichannel_example.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities
        Console 2: ./corhpx apps ctx 2 0 ../examples/libmultichannel_example.so --hpx:hpx=localhost:1338 --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker
        With MPI: mpirun -np 2 ./corhpx apps ctx 2 0 ../examples/libmultichannel_example.so
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
        // cria o recurso multichannel. "partner1" é o primeiro parametro e representa este ponto, os restantes são os outros participantes ("partner2")
        auto channel = domain->CreateLocal<cor::MultiChannel_Client<std::vector<double>>>(domain->Idp(), "",  "partner1", "partner2");
        std::vector<double> vec = {1, 2, 3};
        
        // envia uma mensagem para o parceiro "partner2", com indice 0
        channel->Set(std::move(vec), "partner2", 0);
    }
    // o rank 1 recebe a mensagem
    else {
        // cria o recurso multichannel. "partner2" é o primeiro parametro e representa este ponto, os restantes são os outros participantes ("partner1")
        auto channel = domain->CreateLocal<cor::MultiChannel_Client<std::vector<double>>>(domain->Idp(), "",  "partner2", "partner1");

        // recebe uma mensagem do parceiro "partner1", com indice 0
        auto vec = channel->Get("partner1", 0).get();
        for (auto v : vec) {
            std::cout << v << std::endl;
        }
    }
}
