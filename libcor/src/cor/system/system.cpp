#include "cor/system/system.hpp"

namespace cor {
namespace global {

    //Pod_Client *pod = nullptr;
    std::atomic<unsigned int> pod_index = 0;
    std::map<size_t, unsigned int> active_rscs{};
    // std::map<unsigned int, Pod_Client*> pods;
    std::vector<Pod_Client*> pods;

    hpx::lcos::local::spinlock _mtx;
}}
