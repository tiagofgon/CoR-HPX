#include "cor/services/access_manager.hpp"

namespace cor
{

AccessManager::AccessManager() = default;

AccessManager::AccessManager(std::string const& ctx) :
    _contexts{}
{
    AddContext(ctx);
}

AccessManager::~AccessManager() = default;

void AccessManager::AddContext(std::string const& ctx)
{
    _contexts["ctx"] = 0;
}

int AccessManager::GetPosition(std::string const& ctx)
{
    int pos = _contexts["ctx"];
    std::cout << "contexto: " << ctx << ", return: " << _contexts["ctx"] << std::endl;
    _contexts["ctx"] = _contexts["ctx"] + 1;
    
    return pos;
}

void AccessManager::Reset(std::string const& ctx)
{
    _contexts["ctx"] = 0;
}


}

HPX_REGISTER_COMPONENT_MODULE();

typedef cor::AccessManager AccessManager;
typedef hpx::components::component<AccessManager> server_type;

HPX_REGISTER_COMPONENT(server_type, AccessManager);


typedef cor::AccessManager::AddContext_action_AccessManager AddContext_action_AccessManager;
typedef cor::AccessManager::GetPosition_action_AccessManager GetPosition_action_AccessManager;
typedef cor::AccessManager::Reset_action_AccessManager Reset_action_AccessManager;

HPX_REGISTER_ACTION(AddContext_action_AccessManager);
HPX_REGISTER_ACTION(GetPosition_action_AccessManager);
HPX_REGISTER_ACTION(Reset_action_AccessManager);