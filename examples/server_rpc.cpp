#include <iostream>

#include "cor/cor_hpx.hpp"

extern "C"
{
    void Main(int argc);
    idp_t ServerFunction(idp_t idp);
}

idp_t ServerFunction(idp_t domain_idp)
{
    auto domain = cor::GetDomain();
    auto rsc_idp = domain->Create<cor::ProtoAgent_Client<void(idp_t)>>(domain_idp, "", "libclient_rpc.so", "ClientFunction");
    domain->Run<void(idp_t)>(rsc_idp, rsc_idp);
    domain->Wait<void(idp_t)>(rsc_idp);
    domain->Get<void(idp_t)>(rsc_idp);
    return rsc_idp;
}

void Main(int argc)
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(int)>>(agent_idp);
    auto parent_idp = clos->GetParent();

    auto client_clos_idp = domain->GetPredecessorIdp(parent_idp);
    auto client_domain_idp= domain->GetPredecessorIdp(client_clos_idp);
    auto client_domain = domain->CreateReference<cor::Domain_Client>(client_domain_idp, domain->Idp(), "Client Domain");
    auto global_clos_idp = client_domain->GetIdp("Global Closure");
    auto global_clos = domain->CreateReference<cor::Closure_Client>(global_clos_idp, domain->Idp(), "Global Closure");

    global_clos->Join(agent_idp, "server");
    auto barrier = domain->CreateCollective<cor::Barrier_Client>(global_clos_idp, domain->Idp(), "Barrier", global_clos_idp);
    barrier->Synchronize();
} 
