#include "cor/system/rpc_manager.hpp"

#include "cor/external/crazygaze/rpc/RPCSpreadTransport.h"

namespace cor {

RpcManager::RpcManager(std::string const& id) :
    _id{id},
    _obj{new RPC()},
    _con{nullptr}
{}

RpcManager::~RpcManager() = default;

void RpcManager::StartService()
{
    _con = cz::rpc::SpreadTransport<RPC, RPC>::create(*_obj, _id).get();
}

}
