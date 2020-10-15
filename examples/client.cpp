// #include "cor/cor.hpp"

// extern "C"
// {
//     void Main(int argc, char *argv[]);
// }

// void Main(int argc, char *argv[])
// {
// /*
//     auto domain = cor::GetDomain();

//     auto remote_comm_idp = domain->Spawn("server", 1, "~/placor/examples/libserver.dylib", {}, { "localhost" });
//     auto remote_comm = domain->CreateReference<cor::Communicator>(remote_comm_idp, domain->Idp(), "");

//     auto agent_idp = domain->GetActiveResourceIdp();
//     auto agent = domain->GetLocalResource<cor::Agent<void(int,char**)>>(agent_idp);

// std::cout << "CLIENT: " << agent_idp << std::endl;

//     auto server_msg = agent->Receive();
//     auto remote_domain_idp = server_msg.Get<idp_t>(0);
//     auto remote_agent_idp = server_msg.Get<idp_t>(1);

// std::cout << "CLIENT: remote domain idp -> " << remote_domain_idp << std::endl;
// std::cout << "CLIENT: remote agent idp -> " << remote_agent_idp << std::endl;

//     auto rsc_idp = domain->Create<cor::Agent<void(idp_t)>>(remote_domain_idp, "", "/Users/brunoribeiro/placor/examples/libserver.dylib", "Test");
// std::cout << "<" << rsc_idp << ">" << std::endl;
//     domain->Run<void(idp_t)>(rsc_idp, agent_idp);

// std::cout << "ANTES" << std::endl;
//     auto worker_msg = agent->Receive();
// std::cout << "DEPOIS" << std::endl;
//     auto work = worker_msg.Get<std::string>();
//     std::cout << work << " World" << std::endl;

//     cor::Message stop_msg;
//     agent->Send(remote_agent_idp, stop_msg);
// */
// }
