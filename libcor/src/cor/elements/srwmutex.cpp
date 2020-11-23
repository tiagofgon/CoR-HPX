#include "cor/elements/srwmutex.hpp"

namespace cor {

SRWMutex::SRWMutex() = default;

SRWMutex::SRWMutex(idp_t idp) : 
    _idp{idp} 
{
    //std::cout << "Criado um objeto da classe \"SRWMutex\", com idp: " << _idp << std::endl;
}

SRWMutex::~SRWMutex() = default;

// SRWMutex::SRWMutex(SRWMutex&&) noexcept = default;

// SRWMutex& SRWMutex::operator=(SRWMutex&&) noexcept = default;

void SRWMutex::AcquireWrite()
{
    _mtx.lock();
}

void SRWMutex::ReleaseWrite()
{
    _mtx.unlock();
}

void SRWMutex::AcquireRead()
{
    _mtx.lock_shared();
}

void SRWMutex::ReleaseRead()
{
    _mtx.unlock_shared();
}

}
