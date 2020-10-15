#include "cor/resources/rwmutex.hpp"

namespace cor {

RWMutex::RWMutex() = default;

RWMutex::RWMutex(idp_t idp) : Resource{idp}, SRWMutex{idp} {}

RWMutex::~RWMutex() = default;

RWMutex::RWMutex(RWMutex&&) noexcept = default;

RWMutex& RWMutex::operator=(RWMutex&&) noexcept = default;

}
