// #ifndef COR_RPC_MANAGER_HPP
// #define COR_RPC_MANAGER_HPP

// #include <string>

// #include "cor/external/crazygaze/rpc/RPC.h"

// #include "cor/system/macros.hpp"
// #include "cor/system/rpc_class.hpp"

// namespace cor {

// class RpcManager
// {

// public:
//     explicit RpcManager(std::string const& id);
//     ~RpcManager();

//     void StartService();

//     RpcManager() = delete;
//     RpcManager(RpcManager const&) = delete;
//     RpcManager& operator=(RpcManager const&) = delete;
//     RpcManager(RpcManager&&) noexcept = delete;
//     RpcManager& operator=(RpcManager&&) noexcept = delete;

//     template <typename T, typename ... Args>
//     idp_t Create(idp_t ctx, std::string const& name, std::string const& ctrl, Args&&... args);

//     template <typename T, typename ... Args>
//     void Run(idp_t idp, std::string const& ctrl, Args&&... args);

//     template <typename T>
//     void Wait(idp_t idp, std::string const& ctrl);

//     template <typename T>
//     auto Get(idp_t idp, std::string const& ctrl);

// private:
//     std::string _id;
//     RPC *_obj;
//     std::shared_ptr<cz::rpc::Connection<RPC, RPC>> _con;

// };

// }

// #include "cor/system/rpc_manager.tpp"

// #endif
