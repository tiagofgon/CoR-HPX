// #include <iostream>

// #include "cor/cor.hpp"

// extern "C"
// {
//     void Main(int argc);
//     idp_t ServerFunction(idp_t idp);
// }

// idp_t ServerFunction(idp_t domain_idp)
// {
//     auto domain = cor::GetDomain().get();
//     std::string module = "libclient_rpc.so";
//     std::string function = "ClientFunction";
//     std::string const& modulee = module;
//     std::string const& functionn = function;

//     auto rsc_idp = domain->Create<cor::Agent_Client<void(idp_t)>>(domain_idp, "", modulee, functionn);

//     idp_t const& rsc_idpp = rsc_idp;
//     domain->Run<cor::Agent_Client<void(idp_t)>>(rsc_idpp, rsc_idpp);
//     domain->Wait<cor::Agent_Client<void(idp_t)>>(rsc_idpp);
//     domain->Get<cor::Agent_Client<void(idp_t)>>(rsc_idpp);
//     return rsc_idp;
// }

// void Main(int argc)
// {
//     auto domain = cor::GetDomain().get();
//     auto agent_idp = domain->GetActiveResourceIdp();
//     auto clos_idp = domain->GetPredecessorIdp(agent_idp);
//     auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
//     auto agent = domain->GetLocalResource<cor::Agent_Client<void(int)>>(agent_idp);
//     auto parent_idp = clos->GetParent();

//     auto client_clos_idp = domain->GetPredecessorIdp(parent_idp);
//     auto client_domain_idp= domain->GetPredecessorIdp(client_clos_idp);
//     auto client_domain = domain->CreateReference<cor::Domain_Client>(client_domain_idp, domain->Idp(), "Client Domain");
//     auto global_clos_idp = client_domain->GetIdp("Global Closure");
//     auto global_clos = domain->CreateReference<cor::Closure_Client>(global_clos_idp, domain->Idp(), "Global Closure");

//     global_clos->Join(agent_idp, "server");

//     std::string barrier_name = "Barrier";
//     std::string const& barrier_namee = barrier_name;
//     idp_t const& global_clos_idpp = global_clos_idp;

//     auto barrier = domain->CreateCollective<cor::Barrier_Client>(global_clos_idp, domain->Idp(), barrier_namee, global_clos_idpp);
//     barrier->Synchronize();

// } 
