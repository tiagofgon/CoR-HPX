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
  void clientFunction(idp_t id);
}

void clientFunction(idp_t id)
{
    auto domain = cor::GetDomain();
    std::cout << "From Client: " << id << std::endl;
}

static constexpr std::size_t NUM_AGENTS = 5;

const int spawnedDomains=1;
void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();
 
    auto domain_idp= domain->Idp();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent<void(int,char**)>>(agent_idp);
    auto claus = getClausure();
    auto claus_size = claus->GetTotalMembers();

    auto globalClaus = domain->CreateLocal<cor::Closure>(domain->Idp(), "globalClausura", claus_size + spawnedDomains, agent_idp);
    auto globalClaus_domain_idp = globalClaus->Idp();
    auto server_clos_idp = domain->Spawn("server", spawnedDomains, "~/placor/tests/libserver_rpc.so", {}, { "localhost" });
    auto server_clos = domain->CreateReference<cor::Closure>(server_clos_idp, domain->Idp(), "Server Clausure");
    auto server_domain_idp = domain->GetPredecessorIdp(server_clos_idp);
    
    {               
        auto rsc_idp = domain->Create<cor::Agent<idp_t(idp_t)>>(server_domain_idp, "", "libserver_rpc.so", "serverFunction");
        domain->Run<idp_t(idp_t)>(rsc_idp,domain_idp);
        domain->Wait<idp_t(idp_t)>(rsc_idp);
        auto res = domain->Get<idp_t(idp_t)>(rsc_idp);
        std::cout << "ReturnServer: " << res  << " dClient_idp " << domain_idp << std::endl;
        cor::Message msg;
        agent->Send(0,server_clos_idp , msg);
    }

    globalClaus->Join(agent_idp, "client");
    auto barrierClientServer = domain->CreateCollective<cor::Barrier>(globalClaus_domain_idp, domain->Idp(), "barrier", globalClaus_domain_idp);
    barrierClientServer->Synchronize();
}
