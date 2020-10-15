#include "cor/elements/srwmutex.hpp"

#include "cor/system/system.hpp"
#include "cor/system/pod.hpp"

namespace cor {

SRWMutex::SRWMutex() = default;

SRWMutex::SRWMutex(idp_t idp) : _idp{idp} {}

SRWMutex::~SRWMutex() = default;

SRWMutex::SRWMutex(SRWMutex&&) noexcept = default;

SRWMutex& SRWMutex::operator=(SRWMutex&&) noexcept = default;

void SRWMutex::AcquireWrite()
{
    global::pod->GetConsistencyObject(_idp)->AcquireWrite();
}

void SRWMutex::ReleaseWrite()
{
    global::pod->GetConsistencyObject(_idp)->ReleaseWrite();
}

void SRWMutex::AcquireRead()
{
    global::pod->GetConsistencyObject(_idp)->AcquireRead();
}

void SRWMutex::ReleaseRead()
{
    global::pod->GetConsistencyObject(_idp)->ReleaseRead();
}

}
