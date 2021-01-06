/* ---------- 
-- Tiago Gonçalves - University of Minho, 2021 --

    Programme to be run in parallel with two processes, only. Each process will remotely create an agent in the location of the other process.
    Compile with: --hpx:ini=hpx.component_paths=

Console 1: ./corhpx apps ctx 2 0 ../examples/libcreate_remote.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities --hpx:ini=hpx.component_paths=../examples
Console 2: ./corhpx apps ctx 2 0 ../examples/libcreate_remote.so --hpx:hpx=localhost:1338 --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker --hpx:ini=hpx.component_paths=../examples
With MPI in one console only: mpirun -np 2 ./corhpx apps ctx 2 0 ../examples/libcreate_remote.so --hpx:ini=hpx.component_paths=../examples
---------- */


#include "cor/cor.hpp"

// tem de ser uma função objeto para ser serializada
struct Funcion_object1 {
    void operator()(int i) {
        std::cout << "Function1: " << i << std::endl;
        std::cout << "Locality id: " << hpx::get_locality_id() << std::endl;
    }
};
Funcion_object1 _Function1;
hpx::function<void(int)> Function1(_Function1);


extern "C"
{
    void Main(int argc, char *argv[]);
}

void Main(int argc, char *argv[]) {
    auto domain = cor::GetDomain();
    auto domain_idp = domain->Idp();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(char**)>>(agent_idp);
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto meta_domain_idp = domain->GetPredecessorIdp(domain_idp);
    auto meta_domain = domain->GetLocalResource<cor::Domain_Client>(meta_domain_idp);
    auto member_list = meta_domain->GetMemberList();
    idp_t remote_domain_idp;

    // iteracao pelos dominios que estao no contexto do meta-dominio, para ir buscar o dominio remoto
    for(auto idp : member_list) {
        if(idp != domain_idp && idp != 0) {
            remote_domain_idp = idp;
            break;
        }
    }

    std::cout << "Dominio local: " << domain_idp << std::endl;
    std::cout << "Dominio remoto: " << remote_domain_idp << std::endl;

    // criacao de um novo agente no dominio remoto
    auto remote_agent_idp = domain->Create<cor::Agent_Client<void(int)>>(remote_domain_idp, "", Function1);

    
    if(hpx::get_locality_id() == 0) {
        domain->Run<cor::Agent_Client<void(int)>>(remote_agent_idp, 0).get();
    }
    else {
        domain->Run<cor::Agent_Client<void(int)>>(remote_agent_idp, 1).get();
    }


}

// output:
// o agente que imprimir 0 foi criado remotamente na localidade 1
// o agente que imprimir 1 foi criado remotamente na localidade 0