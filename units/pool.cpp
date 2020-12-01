#include "pool.hpp"

namespace cor
{
    Pool::Pool(std::size_t num_agents) :
        _num_agents{num_agents},
        _fct{nullptr},
        _arg{nullptr},
        barrier{num_agents}
    {
        auto domain = cor::GetDomain();
        auto agent_idp = domain->GetActiveResourceIdp();
        std::string const& path = "";
        auto group = domain->CreateLocal<cor::Group_Client>(domain->Idp(), "", path);
        _group = group->Idp();

        _futures.reserve(num_agents);
        th_ids.reserve(num_agents);
        // for (std::size_t i = 0; i < num_agents; ++i) {
        //     auto agent = domain->CreateLocal<cor::ProtoAgent<void(void*)>>(group->Idp(), "", AgentFunc);
        //     agent->Run((void*)this);
        //     _agents.push_back(agent->Idp());
        // }
    }

    Pool::~Pool()
    {        

    }

    void Pool::Dispatch(void (*funct)(void *), void *arg)
    {
        //this->Run(fct, arg);

        for(int i=0; i<_num_agents; i++) {
            _futures.push_back(
                hpx::async([this, &funct, &arg](){
                    th_ids.push_back(hpx::this_thread::get_id());
                    // std::cout << "thread_id_1: " << hpx::this_thread::get_id() << std::endl;
                    // std::cout << "aux_2: " << aux << std::endl;
                    barrier.wait();
                    funct(arg);
                })
            );
        }
        




    }

    int Pool::GetRank()
    {
        int n;
        int my_rank=-1;
        auto my_id = hpx::this_thread::get_id();
        //std::cout << "thread_id_2: " << my_id << std::endl;
        //std::cout << "aux_3: " << aux << std::endl;
        for(int n=0; n<_num_agents; n++) {
            // std::cout << "my_id: " << my_id << std::endl;
            // std::cout << "fr_id: " << th_ids[n] << std::endl;
            if(th_ids[n] == my_id) {
                my_rank = n;
                break;
            }
        }

        // if(n==_num_agents) my_rank = -1;    // OK, return rank
        // else my_rank = n;     // else, return error
        return my_rank;
        //return th_ids.size();
    }

    void Pool::WaitForIdle()
    {
        hpx::wait_all(_futures);
        //_barrier->WaitForIdle();  
    }


}
