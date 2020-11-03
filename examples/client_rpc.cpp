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
    auto agent = domain->GetLocalResource<cor::ProtoAgent_Client<void(int)>>(agent_idp);
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto clos_size = clos->GetTotalMembers();

    int const& total_spawned = clos_size+spawned_domains;
    auto global_clos = domain->CreateLocal<cor::Closure_Client>(domain->Idp(), "Global Closure", total_spawned, agent_idp);
    auto global_clos_idp = global_clos->Idp();
    auto server_clos_idp = domain->Spawn("server", spawned_domains, "~/placor/cap6/libserver_rpc.so", {}, { "localhost:1338" });
    auto server_clos = domain->CreateReference<cor::Closure_Client>(server_clos_idp, domain->Idp(), "Server Closure");
    auto server_domain_idp = domain->GetPredecessorIdp(server_clos_idp);

    {
        auto rsc_idp = domain->Create<cor::ProtoAgent_Client<idp_t(idp_t)>>(server_domain_idp, "", "libserver_rpc.so", "ServerFunction");
        domain->Run<idp_t(idp_t)>(rsc_idp, domain_idp);
        domain->Wait<idp_t(idp_t)>(rsc_idp);
        auto res = domain->Get<idp_t(idp_t)>(rsc_idp);
        std::cout << "Return From Server: " << res << "\n";
    }

    global_clos->Join(agent_idp, "client");
    auto barrier = domain->CreateCollective<cor::Barrier>(global_clos_idp, domain->Idp(), "Barrier", global_clos_idp);
    barrier->Synchronize();
}
