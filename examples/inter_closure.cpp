#include <iostream>

#include "cor/cor.hpp"

extern "C"
{
    void Main(int input);
}

int spawned_domains = 2;

void Main(int input)
{

    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto clos_size = clos->GetTotalMembers();
    auto rank = clos->GetIdm(agent_idp);
    auto parent_idp = clos->GetParent();

    std::unique_ptr<cor::Closure_Client> global_clos;
    idp_t master_domain_idp;

    if (parent_idp == 0) {
        idp_t const& clos_idpp = clos_idp;
        auto initial_barrier = domain->CreateCollective<cor::Barrier_Client>(clos_idp, domain->Idp(), "Initial Barrier", clos_idpp);
        if (rank == 0) {
            std::string const& path = "/opt/placor-hpx/examples/libcallableModule.so";
            int const& total_spawned = clos_size+spawned_domains;
            idp_t const& agent_idpp = agent_idp;
            global_clos = domain->CreateLocal<cor::Closure_Client>(domain->Idp(), "Global Closure", total_spawned, agent_idpp);
            auto remote_clos_idp = domain->Spawn("ctx2", spawned_domains, "/opt/placor-hpx/examples/libinter_closure.so", {}, { "127.0.0.1" });
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

    // auto master_domain = domain;

    auto master_domain = (parent_idp == 0 && rank == 0) ? domain :
        domain->CreateReference<cor::Domain_Client>(master_domain_idp, domain->Idp(), "Remote Domain");

    auto global_clos_idp = master_domain->GetIdp("Global Closure");
    if (!(parent_idp == 0 && rank == 0))
        global_clos = domain->CreateReference<cor::Closure_Client>(global_clos_idp, domain->Idp(), "Global Closure");
    
    auto name = std::to_string(agent_idp) + ((parent_idp == 0) ? "" : "_s");
    global_clos->Join(agent_idp, name);

    idp_t const& global_clos_idpp = global_clos_idp;
    auto global_barrier = domain->CreateCollective<cor::Barrier_Client>(global_clos_idp, domain->Idp(), "Global Barrier", global_clos_idpp);
    global_barrier->Synchronize();

    if (parent_idp == 0) {
        if (rank == 0)
            std::cout << "Agent:     | Initial Closure: size  rank | Spawned Closure: size  rank | Global Closure: size  rank" << std::endl;
        std::cout << agent_idp << " | " << clos->IdpGlobal() << "       " << clos->GetTotalMembers() << "     " << rank << "    |                             | " << global_clos->IdpGlobal() << "      " << global_clos->GetTotalMembers() << "     " << global_clos->GetIdm(agent_idp) << std::endl;
    } else {
        std::cout << agent_idp << " |                             | " << clos->IdpGlobal() << "       " << clos->GetTotalMembers() << "     " << rank << "    | " << global_clos->IdpGlobal() << "      " << global_clos->GetTotalMembers() << "     " << global_clos->GetIdm(agent_idp) << std::endl;
    }
}
