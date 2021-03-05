
/*
    This example should be run with two localities
    To run: 
    Console 1: ./corhpx apps ctx 2 0 ../examples/libqueue_program.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities --hpx:ini=hpx.component_paths=../examples/
    Console 2: ./corhpx apps ctx 2 0 ../examples/libqueue_program.so --hpx:agas=localhost:1337 --hpx:run-hpx-main  --hpx:worker --hpx:ini=hpx.component_paths=../examples/

    or with MPI: mpirun -np 2 ./corhpx apps ctx 2 0 ../examples/libqueue_program.so --hpx:ini=hpx.component_paths=../examples/
*/

#include "cor/cor.hpp"
#include "queue_interface.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

typedef Queue<Object> queue_type;
REGISTER_DATA(queue_type); // need to register new Data type

void Main(int argc, char *argv[])
{
    if(hpx::get_num_localities().get() != 2) {
        std::cout << "Necessario correr com duas localidades!" << std::endl;
        return;
    }

    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(int,char**)>>(agent_idp);
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto rank = clos->GetIdm(agent_idp);


    if(rank == 0) {
        auto myqueue = domain->CreateLocal<cor::Data_Client<queue_type>>(domain->Idp(), "data");
        Object objA, objB, objC;
        std::cout << "aqui 1" << std::endl;
        myqueue->Run(Push<Object>, objA).get();
        myqueue->Run(Push<Object, Object>, objB, objC).get();
        std::cout << "aqui 2" << std::endl;
        std::vector<idp_t> remote_domains = domain->GetRemoteDomains();
        idp_t dest = remote_domains[0];
        std::cout << "aqui 3" << std::endl;
        myqueue->Migrate(dest);
        std::cout << "aqui 4" << std::endl;
        auto element = myqueue->Run(Pop).get();

        std::cout << myqueue->Run(Size).get() << std::endl; // irá imprimir 2

        std::cout << myqueue->Run(GetId).get() << std::endl; // irá imprimir 42

    }

    auto barrier = domain->CreateCollective<cor::Barrier_Client>(clos_idp, domain->Idp(), "barrier", clos_idp);
    barrier->Synchronize();

}