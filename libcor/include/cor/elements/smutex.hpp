#ifndef COR_SMUTEX_HPP
#define COR_SMUTEX_HPP

#include "cor/system/macros.hpp"

#include <hpx/hpx.hpp>

namespace cor {

class SMutex
{

public:
    SMutex();
    explicit SMutex(idp_t idp);

    ~SMutex();

    // SMutex(const SMutex&) = delete;
    // SMutex& operator=(const SMutex&) = delete;

    // SMutex(SMutex&&) noexcept;
    // SMutex& operator=(SMutex&&) noexcept;

    void Acquire();
    void Release();

private:

    idp_t _idp;

    hpx::lcos::local::mutex _mtx;
    // hpx::lcos::local::condition_variable _cv;
};

}

#endif
