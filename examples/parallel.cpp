#include "cor/cor.hpp"

extern "C"
{
    void Main(int rsc_idp);
}

void Main(int rsc_idp)
{
    auto domain = cor::GetDomain().get();
    auto agent_idp = domain->GetActiveResourceIdp().get();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp).get();
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp).get();
    auto clos_size = clos->GetTotalMembers().get();
    auto rank = clos->GetIdm(agent_idp).get();
    auto parent_idp = clos->GetParent().get();

    std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << "          hpx::locality= " << hpx::get_locality_id() << "\n";
}
