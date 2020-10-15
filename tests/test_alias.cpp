#include <iostream>

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();

    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent<void(int,char**)>>(agent_idp);

    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure>(clos_idp);

    std::cout << "DOMAIN \t\t IDP <" << domain->Idp() << "> ALIAS <" << domain.Idp() << ">" << std::endl;
    std::cout << "CLOSURE \t IDP <" << clos->Idp() << "> ALIAS <" << clos.Idp() << ">" << std::endl;
    std::cout << "AGENT \t\t IDP <" << agent->Idp() << "> ALIAS <" << agent.Idp() << ">" << std::endl;

    auto original_clos_predecessor = domain->GetPredecessorIdp(clos->Idp());
    std::cout << "ORIGINAL CLOS PREDECESSOR -> " << original_clos_predecessor << std::endl;
}
