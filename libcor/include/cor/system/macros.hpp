#ifndef COR_MACROS_HPP
#define COR_MACROS_HPP

#include <cstdint>

using idp_t  = std::uint32_t;
using idm_t  = std::uint32_t;
using page_t = std::uint32_t;

#define COR_REGISTER_TYPE(T) CEREAL_REGISTER_TYPE(T)

namespace cor
{

    static constexpr idp_t MetaDomain = 0;
    static constexpr idp_t MasterDomain = 4294967040;

/*
    static constexpr idp_t MetaDomain = 0;
    static constexpr idp_t MasterDomain = 4294967040;
    static constexpr idp_t ResourceWorld = 4294967039;
    static constexpr idp_t MasterCommunicator = 4294967038;
    static constexpr idp_t MasterAgent = 4294967037;
*/

    // static constexpr std::uint16_t StaticOrganizerMsg = UINT16_MAX;

}

#endif
