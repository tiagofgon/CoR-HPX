#ifndef COR_ACCESS_MANAGER_HPP
#define COR_ACCESS_MANAGER_HPP

#include <hpx/hpx.hpp>

namespace cor
{

// Component to tell how is the first process of a closure
struct AccessManager: public hpx::components::locking_hook< hpx::components::component_base<AccessManager> >
{
public:
    AccessManager();
    AccessManager(std::string const& ctx);
    ~AccessManager();

    void AddContext(std::string const& ctx);
    int GetPosition(std::string const& ctx);
    void Reset(std::string const& ctx);


    HPX_DEFINE_COMPONENT_ACTION(AccessManager, AddContext, AddContext_action_AccessManager);
    HPX_DEFINE_COMPONENT_ACTION(AccessManager, GetPosition, GetPosition_action_AccessManager);
    HPX_DEFINE_COMPONENT_ACTION(AccessManager, Reset, Reset_action_AccessManager);


private:
    std::map<std::string, int> _contexts;

};


}

typedef cor::AccessManager::AddContext_action_AccessManager AddContext_action_AccessManager;
typedef cor::AccessManager::GetPosition_action_AccessManager GetPosition_action_AccessManager;
typedef cor::AccessManager::Reset_action_AccessManager Reset_action_AccessManager;

HPX_REGISTER_ACTION_DECLARATION(AddContext_action_AccessManager);
HPX_REGISTER_ACTION_DECLARATION(GetPosition_action_AccessManager);
HPX_REGISTER_ACTION_DECLARATION(Reset_action_AccessManager);


#endif
