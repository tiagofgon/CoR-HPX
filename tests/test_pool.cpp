#include "cor/cor.hpp"
#include "units/pool.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

void Task(void *p)
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    
    auto group_idp = domain->GetPredecessorIdp(agent_idp);
    auto group = domain->GetLocalResource<cor::Group>(group_idp);

    auto group_size= group->GetTotalMembers();
    auto agent_idm = group->GetIdm(agent_idp);
    
    auto guarda_idp = domain->GetIdp("Guarda");
    auto guarda = domain->GetLocalResource<cor::Mutex>(guarda_idp);

    //auto func = [=] () {std::cout << " agent_idp: " <<  agent_idp << "  agent_idm: " << agent_idm << " group_size: " << group_size << std::endl;};

    guarda->Acquire();
    // func();
    std::cout << " agent_idp: " <<  agent_idp << "  agent_idm: " << agent_idm << " group_size: " << group_size << std::endl;
    guarda->Release();
}

void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();
    auto mutex = domain->CreateLocal<cor::Mutex>(domain->Idp(), "Guarda");

    auto pool = new cor::Pool(10);
    pool->Dispatch(Task, nullptr);
    pool->WaitForIdle();
    delete pool;
}
