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
    _contexts[ctx] = 0;
}

int AccessManager::GetPosition(std::string const& ctx)
{
    int pos = _contexts[ctx];
    std::cout << "contexto: " << ctx << ", return: " << _contexts[ctx] << std::endl;
    _contexts[ctx] = _contexts[ctx] + 1;
    
    return pos;
}

void AccessManager::Reset(std::string const& ctx)
{
    _contexts[ctx] = 0;
}

void AccessManager::AddMyContextLocality(std::string const& ctx, size_t loc)
{
    localities[ctx].push_back(loc);
}

std::vector<size_t> AccessManager::GetContextLocalities(std::string const& ctx)
{
    return localities[ctx];
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



typedef cor::AccessManager::AddMyContextLocality_action_AccessManager AddMyContextLocality_action_AccessManager;
typedef cor::AccessManager::GetContextLocalities_action_AccessManager GetContextLocalities_action_AccessManager;

HPX_REGISTER_ACTION(AddMyContextLocality_action_AccessManager);
HPX_REGISTER_ACTION(GetContextLocalities_action_AccessManager);