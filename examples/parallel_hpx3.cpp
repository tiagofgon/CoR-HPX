// #include "cor/cor.hpp"

// extern "C"
// {
//     void Main(int rsc_idp);
// }

// void Main(int rsc_idp)
// {
//     auto domain = cor::GetDomain().get();
//     auto agent_idp = domain->GetActiveResourceIdp();

//     auto agent = domain->GetLocalResource<cor::Agent_Client<void(int)>>(agent_idp);

//     auto clos_idp = domain->GetPredecessorIdp(agent_idp);
//     auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
//     auto clos_size = clos->GetTotalMembers();
//     auto rank = clos->GetIdm(agent_idp);
//     auto parent_idp = clos->GetParent();

//     std::cout << "troca de mensagens" << std::endl; 

//     if(rank==0) {
//          auto rank_1 = clos->GetIdp(1);
//         // std::cout << "aqui1" << std::endl; 
//         // auto msg = agent->Receive(1, clos_idp);
//         //auto msg = agent->Receive(rank_1);
//         auto msg = agent->Receive();
//         // std::cout << "aqui2" << std::endl; 
//         int res = msg.Get<int>();
//         std::cout << "res: " << res << std::endl;        
//     }
//     else {
//         // hpx::this_thread::sleep_for(std::chrono::seconds(5));
//         cor::Message msg;
//         msg.Add<int>(12345);

//         auto master_idp = clos->GetIdp(0);
//         // auto master_rank = clos->GetIdm(master_idp);
//         // agent->Send(master_rank, clos_idp, msg);

//         agent->Send(master_idp, msg);

//     }

//     std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << "\n";
// }
