#include <iostream>

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

int spawned_domains = 2;

void Main(int argc, char *argv[])
{
    if (argc >=1) { spawned_domains = std::atoi(argv[0]); }

    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure>(clos_idp);
    auto clos_size = clos->GetTotalMembers();
    auto rank = clos->GetIdm(agent_idp);
    auto parent_idp = clos->GetParent();

    cor::ResourcePtr<cor::Closure> global_clos;
    idp_t master_domain_idp;

    if (parent_idp == 0) {
        auto initial_barrier = domain->CreateCollective<cor::Barrier>(clos_idp, domain->Idp(), "Initial Barrier", clos_idp);
        if (rank == 0) {
            global_clos = domain->CreateLocal<cor::Closure>(domain.Idp(), "Global Closure", clos_size+spawned_domains, agent_idp);
            auto remote_clos_idp = domain->Spawn("ctx2", spawned_domains, "~/placor/cap6/libinter_closure.so", {}, { "localhost" });
        } else {
            auto master_idp = clos->GetIdp(0);
            auto master_clos_idp = domain->GetPredecessorIdp(master_idp);
            master_domain_idp = domain->GetPredecessorIdp(master_clos_idp);
        }
        initial_barrier->Synchronize();
    } else {
        auto master_clos_idp = domain->GetPredecessorIdp(parent_idp);
        master_domain_idp = domain->GetPredecessorIdp(master_clos_idp);
    }

    auto master_domain = (parent_idp == 0 && rank == 0) ? domain :
        domain->CreateReference<cor::Domain>(master_domain_idp, domain.Idp(), "Remote Domain");

    auto global_clos_idp = master_domain->GetIdp("Global Closure");
    if (!(parent_idp == 0 && rank == 0))
        global_clos = domain->CreateReference<cor::Closure>(global_clos_idp, domain.Idp(), "Global Closure");
    
    auto name = std::to_string(agent_idp) + ((parent_idp == 0) ? "" : "_s");
    global_clos->Join(agent_idp, name);

    auto global_barrier = domain->CreateCollective<cor::Barrier>(global_clos_idp, domain.Idp(), "Global Barrier", global_clos_idp);
    global_barrier->Synchronize();

    if (parent_idp == 0) {
        if (rank == 0)
            std::cout << "Agent:     | Initial Closure: size  rank | Spawned Closure: size  rank | Global Closure: size  rank" << std::endl;
        std::cout << agent_idp << " | " << clos->Idp() << "       " << clos->GetTotalMembers() << "     " << rank << "    |                             | " << global_clos->Idp() << "      " << global_clos->GetTotalMembers() << "     " << global_clos->GetIdm(agent_idp) << std::endl;
    } else {
        std::cout << agent_idp << " |                             | " << clos->Idp() << "       " << clos->GetTotalMembers() << "     " << rank << "    | " << global_clos->Idp() << "      " << global_clos->GetTotalMembers() << "     " << global_clos->GetIdm(agent_idp) << std::endl;
    }
}
