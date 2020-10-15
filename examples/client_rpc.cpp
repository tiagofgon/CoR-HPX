#include <iostream>

#include "cor/cor_hpx.hpp"

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


    auto rank = clos->GetIdm(agent_idp);
    auto idp_remote_agent = clos->GetIdp(clos_size-rank-1);
    auto rank_agent_remote = clos->GetIdm(idp_remote_agent);
    
    

    std::cout << "Membros da clausura: " << clos_size << "\n";
    std::cout << "Idp da clausura: " << clos_idp << "\n";
    std::cout << "Meu rank: " << rank << "\n";
    std::cout << "Idp do outro membro da clausura: " << idp_remote_agent << "\n";

    // auto global_clos = domain->CreateLocal<cor::Closure_Client>(domain->Idp(), "Global Closure", clos_size+spawned_domains, agent_idp);
    // auto global_clos_idp = global_clos->Idp();
    // auto server_clos_idp = domain->Spawn("server", spawned_domains, "~/placor/cap6/libserver_rpc.so", {}, { "localhost" });
    // auto server_clos = domain->CreateReference<cor::Closure_Client>(server_clos_idp, domain->Idp(), "Server Closure");
    // auto server_domain_idp = domain->GetPredecessorIdp(server_clos_idp);


    auto server_closure_idp = domain->GetPredecessorIdp(idp_remote_agent);
    std::cout << "O idp da clausura no Domain remoto é : " << server_closure_idp << "\n";
    auto server_domain_idp = domain->GetPredecessorIdp(server_closure_idp);
    std::cout << "O idp do Domain remoto é : " << server_domain_idp << "\n";



    // {
    //     rank_domain_remote = 
    //     auto rsc_idp = domain->Create<cor::ProtoAgent_Client<idp_t(idp_t)>>(server_domain_idp, "", "libserver_rpc.so", "ServerFunction");
    //     domain->Run<idp_t(idp_t)>(rsc_idp, domain_idp);
    //     domain->Wait<idp_t(idp_t)>(rsc_idp);
    //     auto res = domain->Get<idp_t(idp_t)>(rsc_idp);
    //     std::cout << "Return From Server: " << res << "\n";
    // }

    // global_clos->Join(agent_idp, "client");
    // auto barrier = domain->CreateCollective<cor::Barrier>(global_clos_idp, domain->Idp(), "Barrier", global_clos_idp);
    // barrier->Synchronize();
}
