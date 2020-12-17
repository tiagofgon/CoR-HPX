// #include "cor/cor.hpp"

// extern "C"
// {
//     void Main(int input);
//     void ClientFunction(idp_t idp);
// }

// void ClientFunction(idp_t idp)
// {
//     std::cout << "From Client: " << idp << std::endl;
// }

// void Main(int input)
// {
//     auto domain = cor::GetDomain().get();
//     auto domain_idp = domain->Idp();
//     auto agent_idp = domain->GetActiveResourceIdp();
//     auto clos_idp = domain->GetPredecessorIdp(agent_idp);
//     auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
//     auto clos_size = clos->GetTotalMembers();
//     auto rank = clos->GetIdm(agent_idp);
//     auto parent_idp = clos->GetParent();
    
//     if(rank == 0) 
//     {
//         std::cout << "Dominio idp: " << domain->Idp() << std::endl;

//         std::string module = "libmodule.so";
//         std::string function = "ServerFunction";
//         std::string const& modulee = module;
//         std::string const& functionn = function;

//         auto remote_agent_idp = clos->GetIdp(1);
//         auto server_clos_idp = domain->GetPredecessorIdp(remote_agent_idp);
//         auto remote_domain_idp = domain->GetPredecessorIdp(server_clos_idp);
//         //auto remote_domain_idp = domain->Idp();

//         std::cout << "remote_domain_idp: " << remote_domain_idp << std::endl;
//         auto rsc_idpp = domain->Create<cor::Agent_Client<idp_t(idp_t)>>(remote_domain_idp, "", modulee, functionn);

//         idp_t const& rsc_idpp_ = rsc_idpp;
//         idp_t const& domain_idpp = domain_idp;
//         domain->Run<cor::Agent_Client<idp_t(idp_t)>>(rsc_idpp, domain_idpp);
//         domain->Wait<cor::Agent_Client<idp_t(idp_t)>>(rsc_idpp_);
//         auto res = domain->Get<cor::Agent_Client<idp_t(idp_t)>>(rsc_idpp_);
//         std::cout << "Return From Server: " << res << "\n";


//         // std::string const& path = "/share/apps/placor-hpx/examples/libmodule.so"; // é necessario fazer estas atribuições porque os argumentos de componentes tem de ser const
//         // auto rsc_idpp = domain->Create<cor::Group_Client>(remote_domain_idp, "group", path);

//         // auto rsc_idpp = domain->Create<cor::Data_Client<idp_t>>(remote_domain_idp, "data");

//     {
//         // auto localities = hpx::find_all_localities();
//         // auto locality = localities[1];
//         // //auto locality = hpx::find_here();
//         // std::cout << "locality: " << locality << "\n";

//         // //auto res = hpx::new_<cor::Group>(locality, 3333, module);

//         // std::string const& bame = "data";
//         // auto res = hpx::new_<cor::Data<idp_t>>(locality, 213123);

//         // //auto res = hpx::new_<cor::Agent<idp_t(idp_t)>>(locality, 213123, modulee, functionn);
//     }

//         std::cout << "---------------------- rsc_idp: " << rsc_idpp << "\n";


//         std::string barrier_name = "Barrier";
//         std::string const& barrier_namee = barrier_name;
//         idp_t const& clos_idpp = clos_idp;

//         auto barrier = domain->CreateCollective<cor::Barrier_Client>(clos_idp, domain->Idp(), barrier_namee, clos_idpp);
//         barrier->Synchronize();

//         // idp_t const& rsc_idpp = rsc_idp;
//         // idp_t const& domain_idpp = domain_idp;
//         // domain->Run<cor::Agent_Client<idp_t(idp_t)>>(rsc_idp, domain_idpp);
//         // domain->Wait<cor::Agent_Client<idp_t(idp_t)>>(rsc_idpp);
//         // auto res = domain->Get<cor::Agent_Client<idp_t(idp_t)>>(rsc_idpp);
//         // std::cout << "Return From Server: " << res << "\n";
//     }

//     if(rank == 1) {
//         std::cout << "Dominio idp: " << domain->Idp() << std::endl;






//         std::string barrier_name = "Barrier";
//         std::string const& barrier_namee = barrier_name;
//         idp_t const& clos_idpp = clos_idp;

//         auto barrier = domain->CreateCollective<cor::Barrier_Client>(clos_idp, domain->Idp(), barrier_namee, clos_idpp);
//         barrier->Synchronize();
//     }






//     std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << "          hpx::locality= " << hpx::get_locality_id() << "\n";
// }
