#ifndef COR_SRWMUTEX_HPP
#define COR_SRWMUTEX_HPP

#include <hpx/hpx.hpp>
#include "cor/system/macros.hpp"

namespace cor {

class SRWMutex
{

public:
    SRWMutex();
    explicit SRWMutex(idp_t idp);

    ~SRWMutex();

    // SRWMutex(const SRWMutex&) = delete;
    // SRWMutex& operator=(const SRWMutex&) = delete;

    // SRWMutex(SRWMutex&&) noexcept;
    // SRWMutex& operator=(SRWMutex&&) noexcept;

    void AcquireRead();
    void ReleaseRead();

    void AcquireWrite();
    void ReleaseWrite();

private:
    idp_t _idp;

    hpx::lcos::local::shared_mutex _mtx;
};

}

#endif
