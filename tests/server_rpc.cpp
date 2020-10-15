#include <iostream>

#include "cor/cor.hpp"

auto getClausure()
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto claus_idp = domain->GetPredecessorIdp(agent_idp);
    auto claus = domain->GetLocalResource<cor::Closure>(claus_idp);
    return claus;
}
  
extern "C"
{
    void Main(int argc, char *argv[]);
    idp_t serverFunction(idp_t  a);
}

static constexpr idm_t MASTER = 0;

idp_t serverFunction(idp_t dClient_idp)
{
    auto domain = cor::GetDomain();
    auto rsc_idp = domain->Create<cor::Agent<void(idp_t)>>(dClient_idp, "", "libclient_rpc.so", "clientFunction");
    domain->Run<void(idp_t)>(rsc_idp, rsc_idp);
    domain->Wait<void(idp_t)>(rsc_idp);
    domain->Get<void(idp_t)>(rsc_idp);
    return dClient_idp;
    //return domain->GetActiveResourceIdp();
}

void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto claus_idp = domain->GetPredecessorIdp(agent_idp);
    auto claus = domain->GetLocalResource<cor::Closure>(claus_idp);
    auto agent = domain->GetLocalResource<cor::Agent<void(int,char**)>>(agent_idp);
    auto parent_idp = claus->GetParent();
    auto client_claus_idp = domain->GetPredecessorIdp(parent_idp);
    auto cliente_domain_idp= domain->GetPredecessorIdp(client_claus_idp);
    auto client_domain= domain->CreateReference<cor::Domain>(cliente_domain_idp, domain->Idp(), "clientDomain");
    auto globalClaus_domain_idp = client_domain->GetIdp("globalClausura");
    auto globalClaus = domain->CreateReference<cor::Closure>(globalClaus_domain_idp, domain->Idp(), "Global Clausura");
    {
        auto msg = agent->Receive(claus->GetParent());
    }
    globalClaus->Join(agent_idp,"server" );
    auto barrierClientServer = domain->CreateCollective<cor::Barrier>(globalClaus_domain_idp, domain->Idp(), "barrier", globalClaus_domain_idp);
    barrierClientServer->Synchronize();
}
