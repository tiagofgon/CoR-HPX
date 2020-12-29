
#include "cor/cor.hpp"


extern "C"
{
    void Main(int input);
}


void Main(int input)
{
    auto domain = cor::GetDomain().get();
    auto agent_idp = domain->GetActiveResourceIdp().get();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp).get();
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp).get();
    auto clos_size = clos->GetTotalMembers().get();
    auto rank = clos->GetIdm(agent_idp).get();
    auto parent_idp = clos->GetParent().get();

    if (parent_idp == 0) {
        auto new_clos_idp = domain->Spawn("ctx2", 2, "libspawn.so", {}, { "localhost" }).get();
        //auto new_clos_idp = domain->Spawn("ctx2", 1, "libspawn.so", {}, { "compute-881-1" });
        // auto new_clos_idp = domain->Spawn("ctx2", 2, "libspawn.so", {}, { "127.0.0.1" });
        //auto new_clos_idp = domain->Spawn("ctx2", 2, "libspawn.so", {}, { "localhost", "compute-881-1" });
        //auto new_clos_idp = domain->Spawn("ctx2", 2, "libspawn.so", {}, { "172.27.7.252" });

        hpx::lcos::barrier barrier("spawn", 2);
        barrier.wait();

        std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << std::endl;
    
    }
    else {
        hpx::lcos::barrier barrier("spawn", 2);
        barrier.wait();

        std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << std::endl;
    }


}