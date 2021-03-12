#ifndef COR_ACCESS_MANAGER_HPP
#define COR_ACCESS_MANAGER_HPP

#include <hpx/hpx.hpp>

namespace cor
{

// Component to discover how is the first process of a closure
struct AccessManager: public hpx::components::locking_hook< hpx::components::component_base<AccessManager> >
{
public:
    AccessManager();
    AccessManager(std::string const& ctx);
    ~AccessManager();

    void AddContext(std::string const& ctx);
    int GetPosition(std::string const& ctx);
    void Reset(std::string const& ctx);


    void AddMyContextLocality(std::string const& ctx, size_t loc);
    std::vector<size_t> GetContextLocalities(std::string const& ctx);

    // saber as portas que estão a ser usadas em cada nó
    void AddConnection(std::string address, unsigned int port);
    // retorna a proxima porta para fazer o spawn
    unsigned int GetNextPort(std::string address);
    void RemoveConnection(std::string address, unsigned int port);


    HPX_DEFINE_COMPONENT_ACTION(AccessManager, AddContext, AddContext_action_AccessManager);
    HPX_DEFINE_COMPONENT_ACTION(AccessManager, GetPosition, GetPosition_action_AccessManager);
    HPX_DEFINE_COMPONENT_ACTION(AccessManager, Reset, Reset_action_AccessManager);

    HPX_DEFINE_COMPONENT_ACTION(AccessManager, AddMyContextLocality, AddMyContextLocality_action_AccessManager);
    HPX_DEFINE_COMPONENT_ACTION(AccessManager, GetContextLocalities, GetContextLocalities_action_AccessManager);

    HPX_DEFINE_COMPONENT_ACTION(AccessManager, AddConnection, AddConnection_action_AccessManager);
    HPX_DEFINE_COMPONENT_ACTION(AccessManager, GetNextPort, GetNextPort_action_AccessManager);
    HPX_DEFINE_COMPONENT_ACTION(AccessManager, RemoveConnection, RemoveConnection_action_AccessManager);

private:
    std::map<std::string, int> _contexts;
    std::map<std::string, std::vector<size_t>> localities;
    std::map<std::string, std::vector<unsigned int>> _connections;
};


}

typedef cor::AccessManager::AddContext_action_AccessManager AddContext_action_AccessManager;
typedef cor::AccessManager::GetPosition_action_AccessManager GetPosition_action_AccessManager;
typedef cor::AccessManager::Reset_action_AccessManager Reset_action_AccessManager;
HPX_REGISTER_ACTION_DECLARATION(AddContext_action_AccessManager);
HPX_REGISTER_ACTION_DECLARATION(GetPosition_action_AccessManager);
HPX_REGISTER_ACTION_DECLARATION(Reset_action_AccessManager);



typedef cor::AccessManager::AddMyContextLocality_action_AccessManager AddMyContextLocality_action_AccessManager;
typedef cor::AccessManager::GetContextLocalities_action_AccessManager GetContextLocalities_action_AccessManager;
HPX_REGISTER_ACTION_DECLARATION(AddMyContextLocality_action_AccessManager);
HPX_REGISTER_ACTION_DECLARATION(GetContextLocalities_action_AccessManager);



typedef cor::AccessManager::AddConnection_action_AccessManager AddConnection_action_AccessManager;
typedef cor::AccessManager::GetNextPort_action_AccessManager GetNextPort_action_AccessManager;
typedef cor::AccessManager::RemoveConnection_action_AccessManager RemoveConnection_action_AccessManager;
HPX_REGISTER_ACTION_DECLARATION(AddConnection_action_AccessManager);
HPX_REGISTER_ACTION_DECLARATION(GetNextPort_action_AccessManager);
HPX_REGISTER_ACTION_DECLARATION(RemoveConnection_action_AccessManager);

#endif
