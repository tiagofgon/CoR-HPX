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
    return;
}

int AccessManager::GetPosition(std::string const& ctx)
{
    int pos = _contexts[ctx];
    //std::cout << "contexto: " << ctx << ", return: " << _contexts[ctx] << std::endl;
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


void AccessManager::AddConnection(std::string address, unsigned int port)
{
    // std::cout << "AddConnection " << address << ":" << port << std::endl; 
    if(_connections.find(address) != _connections.end()) { // se existir o address
        if(find(_connections[address].begin(), _connections[address].end(), port) == _connections[address].end()) { // se já existir o address mas nao existir a porta.
            _connections[address].push_back(port);
        } // else -> Caso a porta já exista não faz nada, para o caso dos spawns, quando um processo spawned arrancar, a sua porta já está inserida
    }
    else { // se não existir o address
        _connections[address].push_back(port);
    }
}
 
unsigned int AccessManager::GetNextPort(std::string address)
{
    if(_connections.find(address) == _connections.end()) {
        _connections[address].push_back(1337);
        return 1337;
    }
    else {
        unsigned int last_port = _connections[address].at(_connections[address].size()-1);
        unsigned int new_port = last_port+1;
        std::vector<unsigned int>::iterator it;

        while(true) {
            it = find(_connections[address].begin(), _connections[address].end(), new_port);
            if(it != _connections[address].end()) { // se a nova porta já existir
                new_port++;
            }
            else {
                break;
            }
        }

        _connections[address].push_back(new_port);
        return new_port;
    }

}

void AccessManager::RemoveConnection(std::string address, unsigned int port)
{
    for(int i=0; i < _connections[address].size(); i++) {
        if(_connections[address][i] == port) {
            _connections[address].erase(_connections[address].begin() + i);
            return;
        }
    }

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



typedef cor::AccessManager::AddConnection_action_AccessManager AddConnection_action_AccessManager;
typedef cor::AccessManager::GetNextPort_action_AccessManager GetNextPort_action_AccessManager;
typedef cor::AccessManager::RemoveConnection_action_AccessManager RemoveConnection_action_AccessManager;
HPX_REGISTER_ACTION(AddConnection_action_AccessManager);
HPX_REGISTER_ACTION(GetNextPort_action_AccessManager);
HPX_REGISTER_ACTION(RemoveConnection_action_AccessManager);