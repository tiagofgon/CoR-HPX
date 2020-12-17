#include "cor/cor.hpp"

extern "C"
{
    void Main(int rsc_idp);
}

void Main(int rsc_idp)
{
    // obter o domínio local
    auto domain = cor::GetDomain().get();

    // obter o idp do agent que está a executar a função de entrada do módulo do utilizador
    auto agent_idp = domain->GetActiveResourceIdp().get();

    // obter uma referência para o agente
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(int,char**)>>(agent_idp).get();

    // criar um grupo para introduzir um novo módulo do utilizador na aplicação
    std::string const& path = "libcallableModule.so"; // é necessario fazer estas atribuições porque os argumentos de componentes tem de ser const
    auto group = domain->CreateLocal<cor::Group_Client>(domain->Idp().get(), "group", path).get();

    // criar um dado no qual irá ser escrito o idp do agente que irá ser criado
    auto data = domain->CreateLocal<cor::Data_Client<idp_t>>(group->Idp().get(), "data").get();
    
    // criar um agente que irá carregar dinamicamente e executar uma função presente no novo módulo do utilizador
    std::string const& func = "Test";
    auto new_agent = domain->CreateLocal<cor::Agent_Client<idp_t(idp_t)>>(group->Idp().get(), "agent", group->GetModuleName().get(), func).get();
    // executa a função solicitada, passando os parâmetros correspondentes
    idp_t const& agent_idpp = agent_idp ;
    new_agent->Run(agent_idpp).get();
    // espera pelo término da execução da função
    new_agent->Wait().get();
    // obtém o resultado da execução da função
    auto res1 = new_agent->Get().get();

    // recebe a mensagem enviado pelo agente criado
    auto msg = agent->Receive().get();
    // obtém o idp presente no conteúdo da mensagem
    auto res2 = msg.Get<idp_t>();

    auto res3 = data->Fetch().get();

    std::cout << res1 << "\t" << res2 << "\t" << res3 << "\n";
}
