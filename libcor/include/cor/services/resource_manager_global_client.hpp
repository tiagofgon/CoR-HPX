
#ifndef COR_RESOURCE_MANAGER_GLOBAL_CLIENT_HPP
#define COR_RESOURCE_MANAGER_GLOBAL_CLIENT_HPP

#include <hpx/hpx.hpp>
#include <hpx/include/actions.hpp>
#include <hpx/include/components.hpp>
#include <hpx/include/lcos.hpp>

#include "cor/services/resource_manager_global.hpp"


namespace cor {

class ResourceManagerGlobal_Client: hpx::components::client_base<ResourceManagerGlobal_Client, ResourceManagerGlobal>
{

	public:
		typedef hpx::components::client_base<ResourceManagerGlobal_Client, ResourceManagerGlobal> base_type;

		ResourceManagerGlobal_Client() : 
			base_type(hpx::new_<cor::ResourceManagerGlobal>(hpx::find_here()))
		{
            std::string basename = "ResourceManagerGlobal_basename";
			if (hpx::get_locality_id() == 0) {
                auto future = hpx::register_with_basename(basename, get_id()).get();
			} 

            resource_manager_global_component = hpx::find_from_basename(basename, 0).get();
            
		}


        void InsertIdp(idp_t idp, hpx::id_type gid) {
            typedef ResourceManagerGlobal::InsertIdp_action_ResourceManagerGlobal action_type;
            return hpx::async<action_type>(resource_manager_global_component, idp, gid).get();
        }

        bool FindIdp(idp_t idp) {
            typedef ResourceManagerGlobal::FindIdp_action_ResourceManagerGlobal action_type;
            return hpx::async<action_type>(resource_manager_global_component, idp).get();
        }

        hpx::id_type GetGidFromIdp(idp_t idp) {
            typedef ResourceManagerGlobal::GetGidFromIdp_action_ResourceManagerGlobal action_type;
            return hpx::async<action_type>(resource_manager_global_component, idp).get();
        }

        void RemoveIdp(idp_t idp) {
            typedef ResourceManagerGlobal::RemoveIdp_action_ResourceManagerGlobal action_type;
            return hpx::async<action_type>(resource_manager_global_component, idp).get();
        }



        void InsertPredecessorIdp(idp_t idp, idp_t idp_predecessor) {
            typedef ResourceManagerGlobal::InsertPredecessorIdp_action_ResourceManagerGlobal action_type;
            return hpx::async<action_type>(resource_manager_global_component, idp, idp_predecessor).get();
        }

        bool FindPredecessorIdp(idp_t idp) {
            typedef ResourceManagerGlobal::FindPredecessorIdp_action_ResourceManagerGlobal action_type;
            return hpx::async<action_type>(resource_manager_global_component, idp).get();
        }

        idp_t GetPredecessorIdp(idp_t idp) {
            typedef ResourceManagerGlobal::GetPredecessorIdp_action_ResourceManagerGlobal action_type;
            return hpx::async<action_type>(resource_manager_global_component, idp).get();
        }

        void RemovePredecessorIdp(idp_t idp) {
            typedef ResourceManagerGlobal::RemovePredecessorIdp_action_ResourceManagerGlobal action_type;
            return hpx::async<action_type>(resource_manager_global_component, idp).get();
        }

        

        void InsertDynamicOrganizer_idps(idp_t idp) {
            typedef ResourceManagerGlobal::InsertDynamicOrganizer_idps_action_ResourceManagerGlobal action_type;
            return hpx::async<action_type>(resource_manager_global_component, idp).get();
        }

        void InsertStaticOrganizer_idps(idp_t idp) {
            typedef ResourceManagerGlobal::InsertStaticOrganizer_idps_action_ResourceManagerGlobal action_type;
            return hpx::async<action_type>(resource_manager_global_component, idp).get();
        }

        bool FindDynamicOrganizer_idps(idp_t idp) {
            typedef ResourceManagerGlobal::FindDynamicOrganizer_idps_action_ResourceManagerGlobal action_type;
            return hpx::async<action_type>(resource_manager_global_component, idp).get();
        }

        bool FindStaticOrganizer_idps(idp_t idp) {
            typedef ResourceManagerGlobal::FindStaticOrganizer_idps_action_ResourceManagerGlobal action_type;
            return hpx::async<action_type>(resource_manager_global_component, idp).get();
        }


	private:
		hpx::id_type resource_manager_global_component;

};

}

#endif
