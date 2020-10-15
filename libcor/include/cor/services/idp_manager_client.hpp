
#ifndef COR_IDP_MANAGER_CLIENT_HPP
#define COR_IDP_MANAGER_CLIENT_HPP

#include <hpx/hpx.hpp>
#include <hpx/include/actions.hpp>
#include <hpx/include/components.hpp>
#include <hpx/include/lcos.hpp>

#include "cor/services/idp_manager.hpp"


namespace cor {

class IdpManager_Client: hpx::components::client_base<IdpManager_Client, IdpManager>
{

	public:
		typedef hpx::components::client_base<IdpManager_Client, IdpManager> base_type;

		IdpManager_Client() : 
			base_type(hpx::new_<cor::IdpManager>(hpx::find_here()))
		{
            std::string basename = "idpManager_basename";
			if (hpx::get_locality_id() == 0) {
                auto future = hpx::register_with_basename(basename, get_id()).get();
                // if(future.get()==true) {
                //     std::cout << "TRUE" << std::endl;
                // } else {
                //     std::cout << "FALSE" << std::endl;
                // }
				
			} 
                // std::cout << "AQUI!" << std::endl;
            idp_manager_component = hpx::find_from_basename(basename, 0).get();
		}

     
        page_t GeneratePage() {
            typedef IdpManager::GeneratePage_action_IdpManager action_type;
            return hpx::async<action_type>(idp_manager_component).get();
        }
        
	private:
		hpx::id_type idp_manager_component;

};

}

#endif
