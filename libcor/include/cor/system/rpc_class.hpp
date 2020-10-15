// #ifndef COR_RPC_CLASS_HPP
// #define COR_RPC_CLASS_HPP

// #include <string>
// #include <future>
// // #include <hpx/hpx.hpp>
// #include "cor/system/macros.hpp"

// namespace cor {

// class RPC
// {

// public:
//     explicit RPC();
//     ~RPC();

//     template <typename T, typename ... Args>
//     idp_t Create(idp_t ctx, std::string const& name, Args&& ... args);

//     template <typename T, typename ... Args>
//     void Run(idp_t idp, Args&&... args);

//     template <typename T>
//     std::future<void> Wait(idp_t idp);

//     template <typename R, typename T>
//     std::future<R> Get(idp_t idp);

// };

// }

// #include "cor/system/rpc_class.tpp"

// #endif
