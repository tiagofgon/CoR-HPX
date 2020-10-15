// #include "cor/cor.hpp"

// extern "C"
// {
//     void Main(int argc, char *argv[]);
// }

// void Main(int argc, char *argv[])
// {
//     // obter o domínio local
//     auto domain = cor::GetDomain();

//     // obter o idp do agent que está a executar a função de entrada do módulo do utilizador
//     auto agent_idp = domain->GetActiveResourceIdp();

//     // obter uma referência para o agente
//     auto agent = domain->GetLocalResource<cor::Agent<void(int,char**)>>(agent_idp);

//     // criar um grupo para introduzir um novo módulo do utilizador na aplicação
//     auto group = domain->CreateLocal<cor::Group>(domain->Idp(), "group", "/opt/placor-hpx/examples/libcallableModule.so");

//     // criar um dado no qual irá ser escrito o idp do agente que irá ser criado
//     auto data = domain->CreateLocal<cor::Data<idp_t>>(group->Idp(), "data");

//     // criar um agente que irá carregar dinamicamente e executar uma função presente no novo módulo do utilizador
//     auto new_agent = domain->CreateLocal<cor::Agent<idp_t(idp_t)>>(group->Idp(), "agent", group->GetModuleName(), "Test");
//     // executa a função solicitada, passando os parâmetros correspondentes
//     new_agent->Run(agent_idp);
//     // espera pelo término da execução da função
//     new_agent->Wait();
//     // obtém o resultado da execução da função
//     auto res1 = new_agent->Get();

//     // recebe a mensagem enviado pelo agente criado
//     auto msg = agent->Receive();
//     // obtém o idp presente no conteúdo da mensagem
//     auto res2 = msg.Get<idp_t>();

//     data->AcquireRead();
//     auto res3 = data->Get();
//     data->ReleaseRead();

//     std::cout << res1 << "\t" << res2 << "\t" << *res3 << "\n";
// }
