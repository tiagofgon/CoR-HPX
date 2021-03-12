/*
    Tiago Gonçalves & António Pina, UM - LIP, 2021

    To run: 
        ./corhpx apps ctx 1 0 ../examples/libspawn.so
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
    auto clos_size = clos->GetTotalMembers();
    auto rank = clos->GetIdm(agent_idp);
    auto parent_idp = clos->GetParent();

    if (parent_idp == 0) {
        auto new_clos_idp = domain->Spawn("ctx2", 2, "libspawn.so", {}, { "localhost" });
        //auto new_clos_idp = domain->Spawn("ctx2", 1, "libspawn.so", {}, { "compute-881-1" });
        // auto new_clos_idp = domain->Spawn("ctx2", 2, "libspawn.so", {}, { "127.0.0.1" });
        //auto new_clos_idp = domain->Spawn("ctx2", 2, "libspawn.so", {}, { "localhost", "compute-881-1" });
        //auto new_clos_idp = domain->Spawn("ctx2", 2, "libspawn.so", {}, { "172.27.7.252" });

        std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << std::endl;
    
    }
    else {
        std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << std::endl;
    }


}