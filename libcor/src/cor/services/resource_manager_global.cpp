#include "cor/services/resource_manager_global.hpp"

namespace cor {

ResourceManagerGlobal::ResourceManagerGlobal() :
    _predecessors{},
    idp_to_gids{},
    dynamicOrganizer_idps{},
    staticOrganizer_idps{}
{}

ResourceManagerGlobal::~ResourceManagerGlobal() = default;

void ResourceManagerGlobal::InsertIdp(idp_t idp, hpx::id_type gid)
{
    idp_to_gids.emplace(idp, gid);
}

bool ResourceManagerGlobal::FindIdp(idp_t idp)
{
    auto it = idp_to_gids.find(idp);
    if ( it == idp_to_gids.end() ) {
       return false;
    } else { 
        return true;
    }
}

hpx::id_type ResourceManagerGlobal::GetGidFromIdp(idp_t idp)
{
    idp_t _idp;
    auto it = idp_to_gids.find(idp);
    if ( it == idp_to_gids.end() ) {
       throw std::runtime_error("Resource " + std::to_string(idp) + " does not exist in ResourceManagerGlobal component!");
    } else { 
        return it->second;
    }
}

void ResourceManagerGlobal::RemoveIdp(idp_t idp)
{
    idp_to_gids.erase(idp);
}







void ResourceManagerGlobal::InsertPredecessorIdp(idp_t idp, idp_t idp_predecessor)
{
    _predecessors.emplace(idp, idp_predecessor);
}

bool ResourceManagerGlobal::FindPredecessorIdp(idp_t idp)
{
    auto it = _predecessors.find(idp);
    if ( it == _predecessors.end() ) {
       return false;
    } else { 
        return true;
    }
}

idp_t ResourceManagerGlobal::GetPredecessorIdp(idp_t idp)
{
    idp_t _idp;
    auto it = _predecessors.find(idp);
    if ( it == _predecessors.end() ) {
       throw std::runtime_error("Resource " + std::to_string(idp) + " does not have a predecessor in ResourceManagerGlobal component!");
    } else { 
        return it->second;
    }
}

void ResourceManagerGlobal::RemovePredecessorIdp(idp_t idp)
{
    _predecessors.erase(idp);
}




void ResourceManagerGlobal::InsertDynamicOrganizer_idps(idp_t idp)
{
    dynamicOrganizer_idps.insert(idp);
}

void ResourceManagerGlobal::InsertStaticOrganizer_idps(idp_t idp)
{
    staticOrganizer_idps.insert(idp);
}

bool ResourceManagerGlobal::FindDynamicOrganizer_idps(idp_t idp)
{
    if(dynamicOrganizer_idps.find(idp) != dynamicOrganizer_idps.end())
        return true;
    return false;
}

bool ResourceManagerGlobal::FindStaticOrganizer_idps(idp_t idp)
{
    if(dynamicOrganizer_idps.find(idp) != dynamicOrganizer_idps.end())
        return true;
    return false;
}



}

// HPX_REGISTER_COMPONENT_MODULE();

typedef cor::ResourceManagerGlobal ResourceManagerGlobal;
typedef hpx::components::component<cor::ResourceManagerGlobal> ResourceManagerGlobal_type;

HPX_REGISTER_COMPONENT(ResourceManagerGlobal_type, ResourceManagerGlobal);




typedef cor::ResourceManagerGlobal::InsertIdp_action_ResourceManagerGlobal InsertIdp_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::FindIdp_action_ResourceManagerGlobal FindIdp_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::GetGidFromIdp_action_ResourceManagerGlobal GetGidFromIdp_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::RemoveIdp_action_ResourceManagerGlobal RemoveIdp_action_ResourceManagerGlobal;

HPX_REGISTER_ACTION(InsertIdp_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION(FindIdp_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION(GetGidFromIdp_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION(RemoveIdp_action_ResourceManagerGlobal);



typedef cor::ResourceManagerGlobal::InsertPredecessorIdp_action_ResourceManagerGlobal InsertPredecessorIdp_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::FindPredecessorIdp_action_ResourceManagerGlobal FindPredecessorIdp_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::GetPredecessorIdp_action_ResourceManagerGlobal GetPredecessorIdp_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::RemovePredecessorIdp_action_ResourceManagerGlobal RemovePredecessorIdp_action_ResourceManagerGlobal;

HPX_REGISTER_ACTION(InsertPredecessorIdp_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION(FindPredecessorIdp_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION(GetPredecessorIdp_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION(RemovePredecessorIdp_action_ResourceManagerGlobal);



typedef cor::ResourceManagerGlobal::InsertDynamicOrganizer_idps_action_ResourceManagerGlobal InsertDynamicOrganizer_idps_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::InsertStaticOrganizer_idps_action_ResourceManagerGlobal InsertStaticOrganizer_idps_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::FindDynamicOrganizer_idps_action_ResourceManagerGlobal FindDynamicOrganizer_idps_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::FindStaticOrganizer_idps_action_ResourceManagerGlobal FindStaticOrganizer_idps_action_ResourceManagerGlobal;

HPX_REGISTER_ACTION(InsertDynamicOrganizer_idps_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION(InsertStaticOrganizer_idps_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION(FindDynamicOrganizer_idps_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION(FindStaticOrganizer_idps_action_ResourceManagerGlobal);