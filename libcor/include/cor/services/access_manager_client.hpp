
#ifndef COR_ACCESS_CLIENT_CLIENT_HPP
#define COR_ACCESS_CLIENT_CLIENT_HPP

#include <hpx/hpx.hpp>

#include "cor/services/access_manager.hpp"

namespace cor {

class AccessManager_Client : hpx::components::client_base<AccessManager_Client, AccessManager>
{

public:
	typedef hpx::components::client_base<AccessManager_Client, AccessManager> base_type;

	AccessManager_Client(std::string ctx) : 
		base_type(hpx::new_<cor::AccessManager>(hpx::find_here(), ctx))
	{
		std::string basename = "AccessManager_basename";
		if (hpx::get_locality_id() == 0) {
			auto future = hpx::register_with_basename(basename, get_id(), 0).get();
			// if(future.get()==true) {
			//     std::cout << "TRUE" << std::endl;
			// } else {
			//     std::cout << "FALSE" << std::endl;
			// }
			
		} 
			// std::cout << "AQUI!" << std::endl;
		access_manager_component = hpx::find_from_basename(basename, 0).get();
	}


	void AddContext(std::string const& ctx)
	{
		typedef AccessManager::AddContext_action_AccessManager action_type;
		return hpx::async<action_type>(access_manager_component, ctx).get();
	}

	int GetPosition(std::string const& ctx)
	{
		typedef AccessManager::GetPosition_action_AccessManager action_type;
		return hpx::async<action_type>(access_manager_component, ctx).get();
	}

	void Reset(std::string const& ctx)
	{
		typedef AccessManager::Reset_action_AccessManager action_type;
		return hpx::async<action_type>(access_manager_component, ctx).get();
	}

	void AddMyContextLocality(std::string const& ctx, size_t loc)
	{
		typedef AccessManager::AddMyContextLocality_action_AccessManager action_type;
		return hpx::async<action_type>(access_manager_component, ctx, loc).get();
	}

	std::vector<size_t> GetContextLocalities(std::string const& ctx)
	{
		typedef AccessManager::GetContextLocalities_action_AccessManager action_type;
		return hpx::async<action_type>(access_manager_component, ctx).get();
	}

	// Returns component's GID
	hpx::id_type GetGid() {
		return access_manager_component;
	}

private:
	hpx::id_type access_manager_component;

};


}

#endif