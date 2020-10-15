#include <iostream>

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
    void TestVoid();
    idp_t TestIdp();
    idp_t Test(idp_t idp);
}

static constexpr idm_t MASTER = 0;

void TestVoid()
{
    auto domain = cor::GetDomain();
    std::cout << "WORKER: " << domain->GetActiveResourceIdp() << std::endl;
}

idp_t TestIdp()
{
    auto domain = cor::GetDomain();
    return domain->GetActiveResourceIdp();
}

idp_t Test(idp_t idp)
{
    return idp;
}

void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();
    
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent<void(int,char**)>>(agent_idp);

    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure>(clos_idp);

    {
        auto msg = agent->Receive(clos->GetParent());
    }
}
