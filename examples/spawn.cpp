#include "cor/cor.hpp"


extern "C"
{
    void Main(int input);
}



void Main(int input)
{
    std::cout << "dentro do modulo1" << std::endl;
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto clos_size = clos->GetTotalMembers();
    auto rank = clos->GetIdm(agent_idp);
    auto parent_idp = clos->GetParent();

    std::cout << "dentro do modulo2" << std::endl;

    if (parent_idp == 0) {
        auto new_clos_idp = domain->Spawn("ctx2", 2, "/opt/placor-hpx/examples/libspawn.so", {}, { "localhost:1338", "localhost:1339" });
    

        std::cout << "PAI" << std::endl;
        // char c;
        // char str1[20];
        // scanf("%s", str1);
        // // scanf("%c", c);
        // std::cout << "dentro do modulo4" << std::endl;

        std::cout << "Antes da barreira: " << std::endl;
        hpx::lcos::barrier barrier("barreira444", 3);
        barrier.wait();
    }

    else {
        std::cout << "FILHO" << std::endl;
        std::cout << "Antes da barreira: " << std::endl;
        hpx::lcos::barrier barrier("barreira444", 3);
        barrier.wait();
    }


    std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << "\n";

}