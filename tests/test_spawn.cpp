#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();
    
    auto agent_idp = domain->GetActiveResourceIdp();
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto comm = domain->GetLocalResource<cor::Closure>(clos_idp);

    if (comm->GetParent() == 0) {
        auto new_clos_idp1 = domain->Spawn("ctx1", 1, "~/placor/tests/libtest_spawn.so", {}, { "localhost" });
        auto new_clos_idp2 = domain->Spawn("ctx2", 1, "~/placor/tests/libtest_spawn.so", {}, { "localhost" });
    }

    //std::cout << domain->Idp() << " -> PARENT: " << comm->GetParent() << std::endl;
}
