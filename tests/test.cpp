#include <iostream>

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
    void Test(idp_t idp);
}

static constexpr idm_t MASTER = 0;

void Test(idp_t idp)
{
    //std::cout << "Hello, World!" << std::endl;
    std::cout << "-->> " << idp << std::endl;
}

void Main(int argc, char *argv[])
{
    // get local domain
    auto domain = cor::GetDomain();

    // get local agent idp and resource
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent<void(int,char**)>>(agent_idp);

    // get agent rank
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure>(clos_idp);
    auto clos_size = clos->GetTotalMembers();
    auto rank = clos->GetIdm(agent_idp);

    if (rank == MASTER) {

        auto group = domain->CreateLocal<cor::Group>(domain->Idp(), "group", "");

        cor::Message msg;
        msg.SetType(0);
        msg.Add<idp_t>(group->Idp());
/*
        for (int i = 0; i < clos_size; ++i)
            agent->Send(clos->GetIdp(i), msg);

        std::cin.ingore();
*/
        agent->Send(clos->GetMemberList(), msg);
    }

    auto msg = agent->Receive();
    auto group_idp = msg.Get<idp_t>();

    std::cout << group_idp << std::endl;
}
