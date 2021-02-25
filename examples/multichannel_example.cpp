/* ---------- 
-- Tiago Gonçalves - University of Minho, 2021 --

---------- */

#include "cor/cor.hpp"


extern "C"
{
    void Main(int argc, char *argv[]);
}

typedef std::vector<std::string> MsgType;

void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(char**)>>(agent_idp);

    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto rank = clos->GetIdm(agent_idp);

    if(clos->GetTotalMembers() != 2) return;

    // rank 0 envia a menagem para o rank 1
    if(rank==0) {
        auto channel = domain->CreateLocal<cor::MultiChannel_Client<std::vector<double>>>(domain->Idp(), "",  "partner1", "partner2");
        std::vector<double> vec = {1, 2, 3};
        channel->Set(std::move(vec), "partner2", 0);
    }
    // o rank 1 recebe a mensagem
    else {
        auto channel = domain->CreateLocal<cor::MultiChannel_Client<std::vector<double>>>(domain->Idp(), "",  "partner2", "partner1");
        auto vec = channel->Get("partner1", 0).get();
        for (auto v : vec) {
            std::cout << v << std::endl;
        }
    }
}
