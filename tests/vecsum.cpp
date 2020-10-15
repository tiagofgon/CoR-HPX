#include <iostream>

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

COR_REGISTER_TYPE(cor::Data<std::vector<int>>);

static constexpr std::size_t ARRAY_SIZE = 5000;
static constexpr idm_t MASTER = 0;

template <typename T>
using Vector = cor::Data<std::vector<T>>;

void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();

    // get local agent idp and resource
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent<void(int,char**)>>(agent_idp);

    // get agent rank
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure>(clos_idp);
    auto clos_size = clos->GetTotalMembers();
    auto rank = clos->GetIdm(agent_idp);

    cor::ResourcePtr<Vector<int>> data;

    if (rank == MASTER) {
        std::vector<int> array;

        for (int i = 1; i <= ARRAY_SIZE; ++i)
            array.push_back(i);

        data = domain->CreateLocal<Vector<int>>(domain->Idp(), "data", std::ref(array));

        cor::Message msg;
        msg.SetType(0);
        msg.Add<idp_t>(data->Idp());

        for (int i = 1; i < clos_size; ++i)
            agent->Send(clos->GetIdp(i), msg);

    } else {

        auto msg = agent->Receive();
        auto data_idp = msg.Get<idp_t>();

        data = domain->CreateReference<Vector<int>>(data_idp, domain->Idp(), "data");
    }

    std::size_t acc = 0;

    data->AcquireRead();
    auto array = data->Get();

    for (int i = rank * (ARRAY_SIZE/clos_size); i < ((rank + 1) * (ARRAY_SIZE/clos_size)); ++i)
        acc += (*array)[i];

    data->ReleaseRead();

    if (rank == MASTER) {
        for (int i = 1; i < clos_size; ++i) {
            auto msg = agent->Receive();
            auto res = msg.Get<std::size_t>();
            acc += res;
        }
        std::cout << "Global Sum -> " << acc << std::endl;
    } else {
        cor::Message msg;
        msg.SetType(1);
        msg.Add<std::size_t>(acc);
        agent->Send(clos->GetIdp(0), msg);
    }
}
