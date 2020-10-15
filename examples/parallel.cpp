// #include "cor/cor.hpp"

// extern "C"
// {
//     void Main(int argc, char *argv[]);
// }

// void Main(int argc, char *argv[])
// {
//     auto domain = cor::GetDomain();
//     auto agent_idp = domain->GetActiveResourceIdp();
//     auto clos_idp = domain->GetPredecessorIdp(agent_idp);
//     auto clos = domain->GetLocalResource<cor::Closure>(clos_idp);
//     auto clos_size = clos->GetTotalMembers();
//     auto rank = clos->GetIdm(agent_idp);
//     auto parent_idp = clos->GetParent();

//     std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << "\n";
// }
