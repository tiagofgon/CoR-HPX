#include <iostream>

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc);
    idp_t ServerFunction(idp_t idp);
}

idp_t ServerFunction(idp_t domain_idp)
{
    auto domain = cor::GetDomain().get();
    std::string const& module = "libclient_rpc.so";
    std::string const& function = "ClientFunction";
    auto rsc_idp = domain->Create<cor::Agent_Client<void(idp_t)>>(domain_idp, "", module, function).get();
    idp_t const& rsc_idpp = rsc_idp;
    domain->Run<cor::Agent_Client<void(idp_t)>>(rsc_idpp, rsc_idpp).get();
    domain->Wait<cor::Agent_Client<void(idp_t)>>(rsc_idpp).get();
    domain->Get<cor::Agent_Client<void(idp_t)>>(rsc_idpp).get();
    return rsc_idp;
    //return 99;
}

void Main(int argc)
{
    auto domain = cor::GetDomain().get();
    auto agent_idp = domain->GetActiveResourceIdp().get();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp).get();
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp).get();
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(int)>>(agent_idp).get();
    auto parent_idp = clos->GetParent().get();

    auto client_clos_idp = domain->GetPredecessorIdp(parent_idp).get();
    auto client_domain_idp= domain->GetPredecessorIdp(client_clos_idp).get();
    auto client_domain = domain->CreateReference<cor::Domain_Client>(client_domain_idp, domain->Idp().get(), "Client Domain").get();
    auto global_clos_idp = client_domain->GetIdp("Global Closure").get();
    auto global_clos = domain->CreateReference<cor::Closure_Client>(global_clos_idp, domain->Idp().get(), "Global Closure").get();

    global_clos->Join(agent_idp, "server").get();

    std::string barrier_name = "Barrier";
    std::string const& barrier_namee = barrier_name;
    idp_t const& global_clos_idpp = global_clos_idp;

    auto barrier = domain->CreateCollective<cor::Barrier_Client>(global_clos_idp, domain->Idp().get(), barrier_namee, global_clos_idpp).get();
    barrier->Synchronize().get();

} 
