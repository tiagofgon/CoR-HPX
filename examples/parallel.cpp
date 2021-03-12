/*
    Tiago Gonçalves & António Pina, UM - LIP, 2021

    To run: 
        Console 1: ./corhpx apps ctx 2 0 ../examples/libparallel.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities
        Console 2: ./corhpx apps ctx 2 0 ../examples/libparallel.so --hpx:hpx=localhost:1338 --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker
        With MPI: mpirun -np N ./corhpx apps ctx N 0 ../examples/libparallel.so
*/

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto clos_size = clos->GetTotalMembers(hpx::launch::async);
    auto rank = clos->GetIdm(hpx::launch::async, agent_idp);
    auto parent_idp = clos->GetParent(hpx::launch::async);

    std::cout << agent_idp << "\t" << rank.get() << "\t" << clos_idp << "\t" << clos_size.get() << "\t" << parent_idp.get() << "\tlocality " << hpx::get_locality_id() << std::endl;
}
