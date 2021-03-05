#ifndef COR_SYSTEM_HPP
#define COR_SYSTEM_HPP

#include <hpx/hpx.hpp>
#include<map>
#include<atomic>
#include<vector>

namespace cor {

class Pod_Client;

namespace global {

    // extern Pod *pod;
    // extern RpcManager *rpc;
    //extern Pod_Client *pod;

    // extern std::atomic<unsigned int> pod_variable = 0;
    // extern std::map<unsigned int, Pod_Client*> pods;

    extern std::atomic<unsigned int> pod_index;
    extern std::map<size_t, unsigned int> active_rscs;
    // extern std::map<unsigned int, Pod_Client*> pods;
    extern std::vector<Pod_Client*> pods;

    extern hpx::lcos::local::spinlock _mtx;
}
}

#endif
