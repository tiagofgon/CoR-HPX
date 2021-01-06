/* ---------- 
-- Tiago Gonçalves - University of Minho, 2021 --
---------- */

#include <iostream>

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
    idp_t ServerFunction(idp_t idp);
}

idp_t ServerFunction(idp_t domain_idp)
{
    auto domain = cor::GetDomain();
    std::string const& module = "libclient_rpc.so";
    std::string const& function = "ClientFunction";
    auto rsc_idp = domain->Create<cor::Agent_Client<void(idp_t)>>(domain_idp, "", module, function);
    domain->Run<cor::Agent_Client<void(idp_t)>>(rsc_idp, rsc_idp);
    return rsc_idp;
}

void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(char**)>>(agent_idp);
    auto parent_idp = clos->GetParent();

    auto client_clos_idp = domain->GetPredecessorIdp(parent_idp);
    auto client_domain_idp= domain->GetPredecessorIdp(client_clos_idp);
    auto client_domain = domain->CreateReference<cor::Domain_Client>(client_domain_idp, domain->Idp(), "Client Domain");
    auto global_clos_idp = client_domain->GetIdp("Global Closure");
    auto global_clos = domain->CreateReference<cor::Closure_Client>(global_clos_idp, domain->Idp(), "Global Closure");

    global_clos->Join(agent_idp, "server");

    std::string barrier_name = "Barrier";
    std::string const& barrier_namee = barrier_name;
    idp_t const& global_clos_idpp = global_clos_idp;

    auto barrier = domain->CreateCollective<cor::Barrier_Client>(global_clos_idp, domain->Idp(), barrier_namee, global_clos_idpp);
    barrier->Synchronize();

} 
