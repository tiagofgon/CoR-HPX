/*
    Tiago Gonçalves & António Pina, UM - LIP, 2021

    To run: 
        ./corhpx apps ctx 1 0 ../examples/libbasicOperations.so
*/


#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

void Main(int argc, char *argv[])
{
    // obter o domínio local
    auto domain = cor::GetDomain();

    // obter o idp do agent que está a executar a função de entrada do módulo do utilizador
    auto agent_idp = domain->GetActiveResourceIdp();

    // obter uma referência para o agente
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(int,char**)>>(agent_idp);

    // criar um grupo para introduzir um novo módulo do utilizador na aplicação
    auto group = domain->CreateLocal<cor::Group_Client>(domain->Idp(), "group", "libcallableModule.so");

    // criar um dado no qual irá ser escrito o idp do agente que irá ser criado
    auto data = domain->CreateLocal<cor::Data_Client<idp_t>>(group->Idp(), "data");

    // criar um agente que irá carregar dinamicamente e executar uma função presente no novo módulo do utilizador
    auto new_agent = domain->CreateLocal<cor::Agent_Client<idp_t(idp_t)>>(group->Idp(), "agent", group->GetModuleName(), "Test");
    
    // executa a função solicitada, passando os parâmetros correspondentes
    auto fut1 = new_agent->Run(agent_idp);
    fut1.wait();
    auto res1 = fut1.get();

    // recebe a mensagem enviado pelo agente criado
    auto msg = agent->Receive();

    // obtém o idp presente no conteúdo da mensagem
    auto res2 = msg.Get<idp_t>();

    // obtem uma copia dos dados albergados por data
    auto res3 = data->Fetch();

    std::cout << res1 << "\t" << res2 << "\t" << res3 << std::endl;
}
