#ifndef COR_GROUP_CLIENT_HPP
#define COR_GROUP_CLIENT_HPP

#include "cor/resources/group.hpp"
#include "cor/elements/dynamic_organizer.hpp"

#include <hpx/hpx.hpp>


namespace cor {

struct Group_Client: hpx::components::client_base<Group_Client, Group>
{

public:
	typedef hpx::components::client_base<Group_Client, Group> base_type;

	friend class hpx::serialization::access;

	typedef DynamicOrganizer organizer;

	// Default construct an empty client side representation (not
	// connected to any existing component). Also needed for serialization
	Group_Client()
	{}

	// Create a client side representation for the existing
	// Closure_Component instance with the given GID
	Group_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Group_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Group_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	// Constructor for replicas
	Group_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Group_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Group_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	/// Standard constructor with parameters
	Group_Client(idp_t idp, std::string const& module) :
		base_type(create_server(idp, module)),
		_idp(idp)
	{}

	Group_Client(idp_t idp, hpx::id_type locality, std::string const& module) :
		base_type(create_server_remote(idp, locality, module)),
		_idp(idp)
	{}


	/** Resource's interface **/
	// method that returns the global idp of the resource, which is present in the class Resource
	idp_t IdpGlobal()
	{
		typedef Resource::Idp_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	// method that returns the GID(hpx::id_type) of this resource locality
	hpx::id_type GetLocalityGID()
	{
		typedef Resource::GetLocalityGID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}
	
	// method that returns the number of this resource locality
	unsigned int GetLocalityID()
	{
		typedef Resource::GetLocalityID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}
	

	/** Dynamic organizer's interface **/
	void Join(idp_t idp, std::string const& name)
	{
		typedef Group::Join_action_Group action_type;
		return hpx::async<action_type>(this->get_id(), idp, name).get();
	}

	void Leave(idp_t idp)
	{
		typedef Group::Leave_action_Group action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}

	std::string GetModuleName()
	{
		typedef Group::GetModuleName_action_Group action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	std::size_t GetTotalMembers()
	{
		typedef Group::GetTotalMembers_action_Group action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	std::vector<idp_t> GetMemberList()
	{
		typedef Group::GetMemberList_action_Group action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

    idp_t GetIdp(idm_t idm)
	{
		typedef Group::GetIdp1_action_Group action_type;
		return hpx::async<action_type>(this->get_id(), idm).get();
	}

    idp_t GetIdp(std::string const& name)
	{
		typedef Group::GetIdp2_action_Group action_type;
		return hpx::async<action_type>(this->get_id(), name).get();
	}

    idm_t GetIdm(idp_t idp)
	{
		typedef Group::GetIdm1_action_Group action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}

    idm_t GetIdm(std::string const& name)
	{
		typedef Group::GetIdm2_action_Group action_type;
		return hpx::async<action_type>(this->get_id(), name).get();
	}

    idm_t GetDynamicOrganizerIdp()
	{
		typedef Group::GetDynamicOrganizerIdp_action_Group action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}


	/** Local Client's interface **/
	// local idp of this resource
	idp_t Idp() {
		return _idp;
	}

	// Returns component's GID
	hpx::id_type GetGid() {
	  return this->get_id();
	}

	int GetComponentType()
	{
		/* Resource identification
		1 - Domain
		2 - Group
		3 - Clousure
		4 - ProtoAgent
		5 - Agent
		6 - Data
		7 - Barrier
		8 - Mutex
		9 - RWMutex
		*/
		return 2;
	}

	// For compilation purposes only, it is never used here!
	hpx::id_type GetMailboxGid() {
		return hpx::find_here();
	}
	

private:
	hpx::future<hpx::id_type> create_server(idp_t idp, std::string const& module) {
		return hpx::local_new<Group>(idp, module);
	}

	hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, std::string const& module) {
		return hpx::new_<Group>(locality, idp, module);
	}

	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
	}

	idp_t _idp; // local idp

};


}


#endif