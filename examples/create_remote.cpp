/* ----- ----- 

    Programa para ser corrido em pararelo com dois processos. Cada processo irá criar remotamente um agente na localidade do outro processo.

 ----- ----- */


#include "cor/cor.hpp"


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
    void Main(int input);
}

void Main(int input) {
    auto domain = cor::GetDomain().get();
    auto domain_idp = domain->Idp().get();
    auto agent_idp = domain->GetActiveResourceIdp().get();
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(int)>>(agent_idp).get();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp).get();
    auto meta_domain_idp = domain->GetPredecessorIdp(domain_idp).get();
    auto meta_domain = domain->GetLocalResource<cor::Domain_Client>(meta_domain_idp).get();
    auto member_list = meta_domain->GetMemberList().get();
    idp_t remote_domain_idp;

    // iteracao pelos dominios que estao no contexto do meta-dominio
    for(auto idp : member_list) {
        if(idp != domain_idp && idp != 0) {
            remote_domain_idp = idp;
            break;
        }
    }

    std::cout << "Dominio local: " << domain_idp << std::endl;
    std::cout << "Dominio remoto: " << remote_domain_idp << std::endl;


    hpx::function<void(int)> const & funcao = Function1;
    // criacao de um novo agente no dominio remoto
    auto remote_agent_idp = domain->Create<cor::Agent_Client<void(int)>>(remote_domain_idp, "", funcao).get();

    
    if(hpx::get_locality_id() == 0) {
        int const& var=0;
        domain->Run<cor::Agent_Client<idp_t(idp_t)>>(remote_agent_idp, var).get();
    }
    else {
        int const& var=1;
        domain->Run<cor::Agent_Client<idp_t(idp_t)>>(remote_agent_idp, var).get();
    }

    domain->Wait<cor::Agent_Client<idp_t(idp_t)>>(remote_agent_idp).get();
    domain->Get<cor::Agent_Client<idp_t(idp_t)>>(remote_agent_idp).get();

}

// output:
// o agente que imprimir 0 foi criado remotamente na localidade 1
// o agente que imprimir 1 foi criado remotamente na localidade 0