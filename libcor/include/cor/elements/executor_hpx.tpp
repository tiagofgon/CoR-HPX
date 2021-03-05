#ifdef COR_EXECUTOR_HPP


#include "cor/system/system.hpp"
#include "cor/system/pod_client.hpp"

namespace cor {

template <typename R, typename ... P>
Executor<R(P...)>::~Executor() = default;

template <typename R, typename ... P>
Executor<R(P...)>::Executor(idp_t idp, unsigned int pod_id, std::function<R(P...)> const& f) :
    _idp{idp},
    _module_name{},
    _function{},
    _f{f},
    _pod_id{pod_id}
{
    //std::cout << "Criado um objeto da classe \"Executor\", com idp: " << _idp << std::endl;
}

template <typename R, typename ... P>
Executor<R(P...)>::Executor(idp_t idp, unsigned int pod_id, std::string const& module, std::string const& function) :
    _idp{idp},
    _module_name{module},
    _function{function},
    _f{},
    _pod_id{pod_id}
{
    //std::cout << "Criado um objeto da classe \"Executor\", com idp: " << _idp << std::endl;
}


template <typename R, typename ... P>
template <typename ... Args>
hpx::future<R> Executor<R(P...)>::Run(Args&&... args)
{
    // std::cout << "RunNow" << std::endl;
    // (std::cout << ... << args) << '\n';

    if (!_function.empty()) {
        _module = dll::DynamicLoader::LoadDynamicLibrary(_module_name);
        _f = _module->LoadFunction<R(P...)>(_function);
    }

    return hpx::async([&, this](){
        // size_t thread_hash = std::hash<std::thread::id>()(std::this_thread::get_id());
        auto id = hpx::threads::get_self_id();
        uint64_t* ptr=(uint64_t*) &id;
        auto id_hpx_thread = (*ptr);
        tid = id_hpx_thread;
        // std::cout << "Thread HPX - Run: " << id_hpx_thread << std::endl;
        // std::cout << "IDP HPX - Run: " << _idp << std::endl;

        if constexpr (std::is_void<R>{}) {
            global::active_rscs[id_hpx_thread] = _pod_id;
            global::pods[_pod_id]->InsertActiveResource(id_hpx_thread, _idp);

            std::cout << "antes execução main - executor" << std::endl;
            std::cout << "_dominio " << _pod_id << "com id_task " << id_hpx_thread << std::endl;

            _f(std::forward<Args>(args)...);

            std::cout << "depois execução main - executor" << std::endl;


            global::pods[_pod_id]->RemoveActiveResource(id_hpx_thread);
            global::active_rscs.erase(id_hpx_thread);
            return; 
        }
        else {
            global::active_rscs[id_hpx_thread] = _pod_id;
            global::pods[_pod_id]->InsertActiveResource(id_hpx_thread, _idp);

            std::cout << "antes execução main - executor" << std::endl;
            std::cout << "_dominio " << _pod_id << "com id_task " << id_hpx_thread << std::endl;

            auto ret = _f(std::forward<Args>(args)...);

            std::cout << "depois execução main - executor" << std::endl;

            global::pods[_pod_id]->RemoveActiveResource(id_hpx_thread);
            global::active_rscs.erase(id_hpx_thread);
            return ret; 
        }



    });
}

template <typename R, typename ... P>
void Executor<R(P...)>::ChangeIdp(idp_t idp)
{
    global::pods[_pod_id]->ChangeActiveResource(tid, idp);
}

template <typename R, typename ... P>  
void Executor<R(P...)>::ResumeIdp()
{
    global::pods[_pod_id]->ChangeActiveResource(tid, _idp);
}

template <typename R, typename ... P>  
idp_t Executor<R(P...)>::CurrentIdp() const
{
    return global::pods[_pod_id]->GetCurrentActiveResource(tid);
}

template <typename R, typename ... P>
idp_t Executor<R(P...)>::OriginalIdp() const
{
    return _idp;
}

template <typename R, typename ... P>
idp_t Executor<R(P...)>::GetExecutorIdp() const {
    return _idp;
}


}


#endif
