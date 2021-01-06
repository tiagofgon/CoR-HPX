/* ---------- 
-- Tiago Gonçalves - University of Minho, 2021 --
---------- */

#include "cor/cor.hpp"

extern "C"
{
    idp_t Test(idp_t rsc_idp);
}

idp_t Test(idp_t rsc_idp)
{
    // obter o domínio local
    auto domain = cor::GetDomain();

    // obter o idp do agent que está a executar uma função preente no módulo do utilizador
    auto agent_idp = domain->GetActiveResourceIdp();

    auto group_idp = domain->GetPredecessorIdp(agent_idp);
    auto group = domain->GetLocalResource<cor::Group_Client>(group_idp);
    auto data_idp = group->GetIdp("data");
    auto data = domain->GetLocalResource<cor::Data_Client<idp_t>>(data_idp);

    data->AcquireWrite();
    auto value = data->Get();
    *value = agent_idp;
    data->ReleaseWrite();

    // obter uma referência para o agente que está a executar a função
    auto agent = domain->GetLocalResource<cor::Agent_Client<idp_t(idp_t)>>(agent_idp);

    // cria uma mensagem e adiciona o idp do agente em questão ao conteúdo da mesma
    cor::Message msg;
    msg.Add<idp_t>(agent_idp);

    // envia a mensagem ao recurso com identificador rsc_idp
    agent->Send(rsc_idp, msg);

    // retorna o idp do agente
    return agent_idp;
}
