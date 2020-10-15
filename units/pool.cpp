#include "pool.hpp"

namespace cor
{
    void AgentFunc(void *arg)
    {
        Pool *p = (Pool*) arg;
        p->PeerAgent();
    }

    Pool::Pool(std::size_t num_agents) :
        _num_agents{num_agents},
        _barrier{new Barrier(num_agents)},
        _fct{nullptr},
        _arg{nullptr},
        _shut{false}
    {
        auto domain = cor::GetDomain();
        auto agent_idp = domain->GetActiveResourceIdp();

        auto group = domain->CreateLocal<cor::Group>(domain->Idp(), "", "");
        _group = group->Idp();

        for (std::size_t i = 0; i < num_agents; ++i) {
            auto agent = domain->CreateLocal<cor::ProtoAgent<void(void*)>>(group->Idp(), "", AgentFunc);
            agent->Run((void*)this);
            _agents.push_back(agent->Idp());
        }
    }

    Pool::~Pool()
    {        
        if (!_shut)
            JoinAgents();
    }

    void Pool::Dispatch(void (*fct)(void*), void *arg)
    {
        _barrier->WaitForIdle();

        {
            std::unique_lock<std::mutex> lk(_mtx);
            _fct = fct;
            _arg = arg;
        }

        _barrier->ReleaseThreads();
    }

    void Pool::WaitForIdle()
    {
        _barrier->WaitForIdle();  
    }

    void Pool::PeerAgent()
    {
        bool my_shut;

        for(;;)
        {
            _barrier->Wait();

            {
                std::unique_lock<std::mutex> lk(_mtx);
                my_shut = _shut;
            }

            if (!my_shut)
                (*(_fct))(_arg);
            else
                break;
        }
    }

    void Pool::JoinAgents()
    {
        {
            std::unique_lock<std::mutex> lk(_mtx);
            _shut = true;
        }

        _barrier->WaitForIdle();
        _barrier->ReleaseThreads();

        // join agents
        auto domain = cor::GetDomain();

        for (auto idp: _agents) {
            auto agent = domain->GetLocalResource<cor::ProtoAgent<void(void*)>>(idp);
            agent->Wait();
            agent->Get();
        }
    }

    Pool::Barrier::Barrier(std::size_t count) :
        _nthreads{count},
        _counter{0},
        _status{true},
        _is_idle{false}
    {}

    Pool::Barrier::~Barrier() = default;

    void Pool::Barrier::Wait()
    {
        bool lstatus;
        
        {
            std::unique_lock<std::mutex> lk(_mtx);
        
            lstatus = _status;
            _counter++;

            if (_counter == _nthreads) {
                _counter = 0;
                _is_idle = true;
                _qidle.notify_all();
            }

            while (lstatus == _status)
                _qtask.wait(lk);
        }
    }

    void Pool::Barrier::WaitForIdle()
    {
        std::unique_lock<std::mutex> lk(_mtx);
        while (!_is_idle)
            _qidle.wait(lk);
    }

    void Pool::Barrier::ReleaseThreads()
    {
        std::unique_lock<std::mutex> lk(_mtx);
        _is_idle = false;
        _status = !_status;
        _qtask.notify_all();
    }
}
