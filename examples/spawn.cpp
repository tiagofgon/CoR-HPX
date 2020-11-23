
#include "cor/cor.hpp"


extern "C"
{
    void Main(int input);
}


void Main(int input)
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto clos_size = clos->GetTotalMembers();
    auto rank = clos->GetIdm(agent_idp);
    auto parent_idp = clos->GetParent();

    if (parent_idp == 0) {
        auto new_clos_idp = domain->Spawn("ctx2", 1, "/home/pg38939/apps/placor-hpx/examples/libspawn.so", {}, { "127.0.0.1" });
    }

    std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << "\n";

}