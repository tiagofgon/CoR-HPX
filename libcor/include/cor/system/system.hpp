#ifndef COR_SYSTEM_HPP
#define COR_SYSTEM_HPP
#include <hpx/hpx.hpp>

namespace cor {

// class Pod;
// class RpcManager;
class Pod_Client;

namespace global {

    // extern Pod *pod;
    // extern RpcManager *rpc;
    extern Pod_Client *pod;
}
}

#endif
