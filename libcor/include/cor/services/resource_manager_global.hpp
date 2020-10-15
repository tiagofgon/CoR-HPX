#ifndef COR_RESOURCE_MANAGER_GLOBAL_HPP
#define COR_RESOURCE_MANAGER_GLOBAL_HPP

#include <hpx/hpx.hpp>
#include <hpx/include/actions.hpp>
#include <hpx/include/components.hpp>

#include "cor/system/macros.hpp"


namespace cor {

// Garante a sincronização entre actions
struct ResourceManagerGlobal : public hpx::components::locking_hook<hpx::components::component_base<ResourceManagerGlobal>>
{

public:
    ~ResourceManagerGlobal();
    ResourceManagerGlobal();

    void InsertIdp(idp_t idp, hpx::id_type gid);
    bool FindIdp(idp_t idp);
    hpx::id_type GetGidFromIdp(idp_t idp);
    void RemoveIdp(idp_t idp);

    HPX_DEFINE_COMPONENT_ACTION(ResourceManagerGlobal, InsertIdp, InsertIdp_action_ResourceManagerGlobal);
    HPX_DEFINE_COMPONENT_ACTION(ResourceManagerGlobal, FindIdp, FindIdp_action_ResourceManagerGlobal);
    HPX_DEFINE_COMPONENT_ACTION(ResourceManagerGlobal, GetGidFromIdp, GetGidFromIdp_action_ResourceManagerGlobal);
    HPX_DEFINE_COMPONENT_ACTION(ResourceManagerGlobal, RemoveIdp, RemoveIdp_action_ResourceManagerGlobal);


    void InsertPredecessorIdp(idp_t idp, idp_t idp_predecessor);
    bool FindPredecessorIdp(idp_t idp);
    idp_t GetPredecessorIdp(idp_t idp);
    void RemovePredecessorIdp(idp_t idp);

    HPX_DEFINE_COMPONENT_ACTION(ResourceManagerGlobal, InsertPredecessorIdp, InsertPredecessorIdp_action_ResourceManagerGlobal);
    HPX_DEFINE_COMPONENT_ACTION(ResourceManagerGlobal, FindPredecessorIdp, FindPredecessorIdp_action_ResourceManagerGlobal);
    HPX_DEFINE_COMPONENT_ACTION(ResourceManagerGlobal, GetPredecessorIdp, GetPredecessorIdp_action_ResourceManagerGlobal);
    HPX_DEFINE_COMPONENT_ACTION(ResourceManagerGlobal, RemovePredecessorIdp, RemovePredecessorIdp_action_ResourceManagerGlobal);



    void InsertDynamicOrganizer_idps(idp_t idp);
    void InsertStaticOrganizer_idps(idp_t idp);
    bool FindDynamicOrganizer_idps(idp_t idp);
    bool FindStaticOrganizer_idps(idp_t idp);

    HPX_DEFINE_COMPONENT_ACTION(ResourceManagerGlobal, InsertDynamicOrganizer_idps, InsertDynamicOrganizer_idps_action_ResourceManagerGlobal);
    HPX_DEFINE_COMPONENT_ACTION(ResourceManagerGlobal, InsertStaticOrganizer_idps, InsertStaticOrganizer_idps_action_ResourceManagerGlobal);
    HPX_DEFINE_COMPONENT_ACTION(ResourceManagerGlobal, FindDynamicOrganizer_idps, FindDynamicOrganizer_idps_action_ResourceManagerGlobal);
    HPX_DEFINE_COMPONENT_ACTION(ResourceManagerGlobal, FindStaticOrganizer_idps, FindStaticOrganizer_idps_action_ResourceManagerGlobal);

private:
    std::map<idp_t, hpx::id_type> idp_to_gids; // Stores all idps and gids of the program
    std::map<idp_t, idp_t> _predecessors; // o segundo é o predecessor do primeiro

    std::set<idp_t> dynamicOrganizer_idps;
    std::set<idp_t> staticOrganizer_idps;
};

}

typedef cor::ResourceManagerGlobal::InsertIdp_action_ResourceManagerGlobal InsertIdp_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::FindIdp_action_ResourceManagerGlobal FindIdp_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::GetGidFromIdp_action_ResourceManagerGlobal GetGidFromIdp_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::RemoveIdp_action_ResourceManagerGlobal RemoveIdp_action_ResourceManagerGlobal;

HPX_REGISTER_ACTION_DECLARATION(InsertIdp_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION_DECLARATION(FindIdp_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION_DECLARATION(GetGidFromIdp_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION_DECLARATION(RemoveIdp_action_ResourceManagerGlobal);



typedef cor::ResourceManagerGlobal::InsertPredecessorIdp_action_ResourceManagerGlobal InsertPredecessorIdp_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::FindPredecessorIdp_action_ResourceManagerGlobal FindPredecessorIdp_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::GetPredecessorIdp_action_ResourceManagerGlobal GetPredecessorIdp_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::RemovePredecessorIdp_action_ResourceManagerGlobal RemovePredecessorIdp_action_ResourceManagerGlobal;

HPX_REGISTER_ACTION_DECLARATION(InsertPredecessorIdp_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION_DECLARATION(FindPredecessorIdp_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION_DECLARATION(GetPredecessorIdp_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION_DECLARATION(RemovePredecessorIdp_action_ResourceManagerGlobal);




typedef cor::ResourceManagerGlobal::InsertDynamicOrganizer_idps_action_ResourceManagerGlobal InsertDynamicOrganizer_idps_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::InsertStaticOrganizer_idps_action_ResourceManagerGlobal InsertStaticOrganizer_idps_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::FindDynamicOrganizer_idps_action_ResourceManagerGlobal FindDynamicOrganizer_idps_action_ResourceManagerGlobal;
typedef cor::ResourceManagerGlobal::FindStaticOrganizer_idps_action_ResourceManagerGlobal FindStaticOrganizer_idps_action_ResourceManagerGlobal;

HPX_REGISTER_ACTION_DECLARATION(InsertDynamicOrganizer_idps_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION_DECLARATION(InsertStaticOrganizer_idps_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION_DECLARATION(FindDynamicOrganizer_idps_action_ResourceManagerGlobal);
HPX_REGISTER_ACTION_DECLARATION(FindStaticOrganizer_idps_action_ResourceManagerGlobal);

#endif
