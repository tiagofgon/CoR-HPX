#ifdef COR_EXECUTOR_HPP


#include "cor/system/system.hpp"
#include "cor/system/pod_client.hpp"

namespace cor {

template <typename R, typename ... P>
Executor<R(P...)>::~Executor() = default;

template <typename R, typename ... P>
Executor<R(P...)>::Executor(idp_t idp, std::function<R(P...)> const& f) :
    _idp{idp},
    _module_name{},
    _function{},
    _f{f}
{
    //std::cout << "Criado um objeto da classe \"Executor\", com idp: " << _idp << std::endl;
}

template <typename R, typename ... P>
Executor<R(P...)>::Executor(idp_t idp, std::string const& module, std::string const& function) :
    _idp{idp},
    _module_name{module},
    _function{function},
    _f{}
{
    //std::cout << "Criado um objeto da classe \"Executor\", com idp: " << _idp << std::endl;
}


// template <typename R, typename ... P>
// Executor<R(P...)>::Executor(Executor<R(P...)>&&) noexcept = default;

// template <typename R, typename ... P>
// Executor<R(P...)>& Executor<R(P...)>::operator=(Executor<R(P...)>&&) noexcept = default;


// template <typename R, typename ... P>
// template <typename ... Args>
// hpx::future<R> Executor<R(P...)>::Run(Args... args)
// {
//     // std::cout << "RunNow" << std::endl;
//     // (std::cout << ... << args) << '\n';

//     if (!_function.empty()) {
//         _module = dll::DynamicLoader::LoadDynamicLibrary(_module_name);
//         _f = _module->LoadFunction<R(P...)>(_function);
//     }

//     return hpx::async([&, this](){
//         // size_t thread_hash = std::hash<std::thread::id>()(std::this_thread::get_id());
//         auto id = hpx::threads::get_self_id();
//         uint64_t* ptr=(uint64_t*) &id;
//         auto id_hpx_thread = (*ptr);
//         tid = id_hpx_thread;
//         // std::cout << "Thread HPX - Run: " << id_hpx_thread << std::endl;
//         // std::cout << "IDP HPX - Run: " << _idp << std::endl;

//         if constexpr (std::is_void<R>{}) {
//             global::pod->InsertActiveResource(id_hpx_thread, _idp);
//             _f(args...);
//             global::pod->RemoveActiveResource(id_hpx_thread);
//             return; 
//         }
//         else {
//             global::pod->InsertActiveResource(id_hpx_thread, _idp);
//             auto ret = _f(args...);
//             global::pod->RemoveActiveResource(id_hpx_thread);
//             return ret; 
//         }
//     });
// }

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
            global::pod->InsertActiveResource(id_hpx_thread, _idp);
            _f(std::forward<Args>(args)...);
            global::pod->RemoveActiveResource(id_hpx_thread);
            return; 
        }
        else {
            global::pod->InsertActiveResource(id_hpx_thread, _idp);
            auto ret = _f(std::forward<Args>(args)...);
            global::pod->RemoveActiveResource(id_hpx_thread);
            return ret; 
        }
    });
}

template <typename R, typename ... P>
void Executor<R(P...)>::ChangeIdp(idp_t idp)
{
    global::pod->ChangeActiveResource(tid, idp);
}

template <typename R, typename ... P>  
void Executor<R(P...)>::ResumeIdp()
{
    global::pod->ChangeActiveResource(tid, _idp);
}

template <typename R, typename ... P>  
idp_t Executor<R(P...)>::CurrentIdp() const
{
    return global::pod->GetCurrentActiveResource(tid);
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
