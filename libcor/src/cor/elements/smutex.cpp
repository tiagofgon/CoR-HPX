#include "cor/elements/smutex.hpp"

#include "cor/system/system.hpp"

namespace cor {

SMutex::SMutex() = default;

SMutex::SMutex(idp_t idp) : 
    _idp{idp} 
{
    
}

SMutex::~SMutex() = default;

// SMutex::SMutex(SMutex&&) noexcept = default;

// SMutex& SMutex::operator=(SMutex&&) noexcept = default;

void SMutex::Acquire()
{
    _mtx.lock();
}

void SMutex::Release()
{
    _mtx.unlock();
}

}
