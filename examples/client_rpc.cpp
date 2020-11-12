#include <iostream>

#include "cor/cor.hpp"

extern "C"
{ 
    void Main(int argc);
    void ClientFunction(idp_t idp);
}

void ClientFunction(idp_t idp)
{
    std::cout << "From Client: " << idp << std::endl;
}

const int spawned_domains = 1;

void Main(int argc)
{ 
    auto domain = cor::GetDomain();

    auto domain_idp = domain->Idp();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(int)>>(agent_idp);
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto clos_size = clos->GetTotalMembers();

    int const& total_spawned = clos_size+spawned_domains;
    idp_t const& agent_idpp = agent_idp;
    auto global_clos = domain->CreateLocal<cor::Closure_Client>(domain->Idp(), "Global Closure", total_spawned, agent_idpp);
    auto global_clos_idp = global_clos->Idp();
    auto server_clos_idp = domain->Spawn("server", spawned_domains, "/opt/placor-hpx/examples/libserver_rpc.so", {}, { "127.0.0.1" });
    auto server_clos = domain->CreateReference<cor::Closure_Client>(server_clos_idp, domain->Idp(), "Server Closure");
    auto server_domain_idp = domain->GetPredecessorIdp(server_clos_idp);

    {
        std::cout << "ENTREI NO BLOCO" << "\n";
        std::string module = "libserver_rpc.so";
        std::string function = "ServerFunction";
        std::string const& modulee = module;
        std::string const& functionn = function;
        auto rsc_idp = domain->Create<cor::Agent_Client<idp_t(idp_t)>>(server_domain_idp, "", modulee, functionn);
        std::cout << "---------------------- rsc_idp: " << rsc_idp << "\n";
        idp_t const& rsc_idpp = rsc_idp;
        idp_t const& domain_idpp = domain_idp;
        domain->Run<cor::Agent_Client<idp_t(idp_t)>>(rsc_idp, domain_idpp);
        domain->Wait<cor::Agent_Client<idp_t(idp_t)>>(rsc_idpp);
        auto res = domain->Get<cor::Agent_Client<idp_t(idp_t)>>(rsc_idpp);
        std::cout << "Return From Server: " << res << "\n";
    }

    global_clos->Join(agent_idpp, "client");
    std::string barrier_name = "Barrier";
    std::string const& barrier_namee = barrier_name;
    idp_t const& global_clos_idpp = global_clos_idp;
    auto barrier = domain->CreateCollective<cor::Barrier_Client>(global_clos_idp, domain->Idp(), barrier_namee, global_clos_idpp);
    barrier->Synchronize();
}
