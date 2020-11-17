#ifndef COR_EXECUTOR_HPP
#define COR_EXECUTOR_HPP

#include <thread>
#include <future>
#include <atomic>
#include <functional>

#include "cor/system/macros.hpp"
#include "cor/external/dll/dll.hpp"

#include <hpx/hpx.hpp>


namespace cor {

template <typename> class Executor;

template <typename R, typename ... P>
class Executor<R(P...)>
{

friend class hpx::serialization::access;
// friend class ProtoAgent<R(P...)>;

public:
    ~Executor();

// protected:
    Executor() = delete;
    Executor(idp_t idp, std::function<R(P...)> const& f);
    Executor(idp_t idp, std::string const& module, std::string const& function);

    // Executor(const Executor&) = delete;
    // Executor& operator=(const Executor&) = delete;

    // Executor(Executor&&) noexcept;
    // Executor& operator=(Executor&&) noexcept;

    template <typename ... Args>
    void Run(Args&&... args);

    void Wait();
    R Get();

    void ChangeIdp(idp_t idp);
    void ResumeIdp();

    idp_t CurrentIdp() const;
    idp_t OriginalIdp() const;

    idp_t GetExecutorIdp() const;


private:
    idp_t _idp;
    std::string _module_name;
    std::string _function;
    std::function<R(P...)> _f;

    dll::DynamicLibrary* _module;

    hpx::future<R> _future_hpx;
    std::uint64_t tid;

};


}


#include "cor/elements/executor_hpx.tpp"


#endif
