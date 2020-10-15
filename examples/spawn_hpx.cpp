
// #include <hpx/hpx.hpp>
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
//     std::cout << "Localidade:" << hpx::get_locality_id() << std::endl;

//     if (parent_idp == 0)
//         auto new_clos_idp = domain->Spawn("ctx2", 2, "/opt/placor-hpx/examples/libspawn_hpx.so", {}, { "localhost" });

//     std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << "\n";

// }