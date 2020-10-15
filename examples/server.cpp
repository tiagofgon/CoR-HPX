// #include "cor/cor.hpp"

// extern "C"
// {
//     void Main(int argc, char *argv[]);
//     void Test(idp_t rsc_idp);
// }

// void Test(idp_t rsc_idp)
// {
//     auto domain = cor::GetDomain();
//     auto agent_idp = domain->GetActiveResourceIdp();
//     auto agent = domain->GetLocalResource<cor::Agent<void(idp_t)>>(agent_idp);
//     cor::Message msg;
//     msg.Add<std::string>("Hello");
//     agent->Send(rsc_idp, msg);
// }

// void Main(int argc, char *argv[])
// {
//     auto domain = cor::GetDomain();

//     auto agent_idp = domain->GetActiveResourceIdp();
//     auto agent = domain->GetLocalResource<cor::Agent<void(int,char**)>>(agent_idp);

//     auto clos_idp = domain->GetPredecessorIdp(agent_idp);
//     auto clos = domain->GetLocalResource<cor::Closure>(clos_idp);

// std::cout << "DOMAIN: " << domain->Idp() << std::endl;
// std::cout << "CLOSURE: " << clos_idp << std::endl;
// std::cout << "SERVER: " << agent_idp << std::endl;

//     cor::Message msg;
//     msg.Add<idp_t>(domain->Idp());
//     msg.Add<idp_t>(agent_idp);
//     agent->Send(clos->GetParent(), msg);

//     auto rmsg = agent->Receive();
// }
