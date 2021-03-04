// Tiago Gonçalves & António Pina - University of Minho - LIP, 2021
//
// Program to be run in parallel with two pods, only.
// Each pod will remotely create an agent in the location of the other pod.
//    Compile with: --hpx:ini=hpx.component_paths= "location of the example"
//
// Console 1: ./corhpx apps ctx 2 1 0 ../examples/libcreate_remote.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities --hpx:ini=hpx.component_paths=../examples
// Console 2: ./corhpx apps ctx 2 1 0 ../examples/libcreate_remote.so --hpx:hpx=localhost:1338 --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker --hpx:ini=hpx.component_paths=../examples
// 
// With MPI in one console only: mpirun -np 2 ./corhpx apps ctx 2 1 0 ../examples/libcreate_remote.so --hpx:ini=hpx.component_paths=../examples
//
// With virtual pods: ./corhpx apps ctx 1 2 0 ../examples/libcreate_remote.so
//

#include "cor/cor.hpp"

// tem de ser uma função objeto para ser serializada
struct funcion_object {
    void operator()(idp_t remote_idp) {
        auto domain = cor::GetDomain();
        auto domain_idp = domain->Idp();
        std::cout << "Function spawned from " << remote_idp << " executed on domain " << domain_idp << "" << std::endl;
    }
};
hpx::function<void(idp_t)> Function = funcion_object();

extern "C"
{
    void Main(int argc, char *argv[]);
}

void Main(int argc, char *argv[]) {
    auto domain = cor::GetDomain();
    auto domain_idp = domain->Idp();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::ProtoAgent_Client<void(char**)>>(agent_idp);
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto rank = clos->GetIdm(agent_idp);

    auto remote_domains = domain->GetRemoteDomains();
    idp_t remote_domain_idp = remote_domains[0];

    // criacao de um novo agente no dominio remoto
    auto remote_agent_idp = domain->Create<cor::ProtoAgent_Client<void(idp_t)>>(remote_domain_idp, "", Function);

    if(rank == 0) {
        domain->Run<cor::ProtoAgent_Client<void(idp_t)>>(remote_agent_idp, domain_idp).get();
    }
    else {
        domain->Run<cor::ProtoAgent_Client<void(idp_t)>>(remote_agent_idp, domain_idp).get();
    }
}

// output:
// Function spawned from x executed on domain y
// Function spawned from y executed on domain x