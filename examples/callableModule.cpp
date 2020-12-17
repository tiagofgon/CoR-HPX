#include "cor/cor.hpp"

extern "C"
{
    idp_t Test(idp_t rsc_idp);
}

idp_t Test(idp_t rsc_idp)
{
    // obter o domínio local
    auto domain = cor::GetDomain().get();

    // obter o idp do agent que está a executar uma função preente no módulo do utilizador
    auto agent_idp = domain->GetActiveResourceIdp().get();

    auto group_idp = domain->GetPredecessorIdp(agent_idp).get();
    auto group = domain->GetLocalResource<cor::Group_Client>(group_idp).get();
    auto data_idp = group->GetIdp("data").get();
    auto data = domain->GetLocalResource<cor::Data_Client<idp_t>>(data_idp).get();

    data->Acquire().get();
    // auto value = *data;
    auto value = data->Get();
    *value = agent_idp;
    data->Release().get();

    // obter uma referência para o agente que está a executar a função
    auto agent = domain->GetLocalResource<cor::Agent_Client<idp_t(idp_t)>>(agent_idp).get();

    // cria uma mensagem e adiciona o idp do agente em questão ao conteúdo da mesma
    cor::Message msg;
    msg.Add<idp_t>(agent_idp);

    // envia a mensagem ao recurso com identificador rsc_idp
    agent->Send(rsc_idp, msg).get();

    // retorna o idp do agente
    return agent_idp;
}
