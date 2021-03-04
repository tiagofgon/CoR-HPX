#ifndef COR_CLOSURE_CLIENT_HPP
#define COR_CLOSURE_CLIENT_HPP

#include "cor/resources/closure.hpp"

#include <hpx/hpx.hpp>


namespace cor {

class Domain_Client;
class Pod_Client;

class Closure_Client : hpx::components::client_base<Closure_Client, Closure>
{

public:
	typedef hpx::components::client_base<Closure_Client, Closure> base_type;

	friend class hpx::serialization::access;

	typedef StaticOrganizer organizer;

	// Default construct an empty client side representation (not
	// connected to any existing component). Also needed for serialization
	Closure_Client()
	{}

	// Create a client side representation for the existing
	// Closure instance with the given GID
	Closure_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal()),
		_total_members(GetFixedTotalMembers())
	{}

	Closure_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal()),
		_total_members(GetFixedTotalMembers())
	{}

	Closure_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal()),
		_total_members(GetFixedTotalMembers())
	{}

	// Constructor for replicas
	Closure_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp),
		_total_members(GetFixedTotalMembers())
	{}

	Closure_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp),
		_total_members(GetFixedTotalMembers())
	{}

	Closure_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp),
		_total_members(GetFixedTotalMembers())
	{}

	/// Standard constructor with parameters
	Closure_Client(idp_t idp, unsigned int total_members, idp_t parent) :
		base_type(create_server(idp, total_members, parent)),
		_idp(idp),
		_total_members(total_members)
	{}

	Closure_Client(idp_t idp, hpx::id_type locality, unsigned int total_members, idp_t parent) :
		base_type(create_server_remote(idp, locality, total_members, parent)),
		_idp(idp),
		_total_members(total_members)
	{}


	Closure_Client(idp_t idp, unsigned int pod_id, unsigned int total_members, idp_t parent) :
		base_type(create_server(idp, total_members, parent)),
		_idp(idp),
		_total_members(total_members)
	{}











	/** Resource's interface **/
	// method that returns the global idp of the resource, which is present in the class Resource
	hpx::future<idp_t> IdpGlobal(hpx::launch::async_policy)
	{
		typedef Resource::Idp_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id());
	}

	idp_t IdpGlobal()
	{
		typedef Resource::Idp_action_Resource action_type;
		return action_type()(base_type::get_id());
	}

	// method that returns the GID(hpx::id_type) of this resource locality
	hpx::future<hpx::id_type> GetLocalityGID(hpx::launch::async_policy)
	{
		typedef Resource::GetLocalityGID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id());
	}

	hpx::id_type GetLocalityGID()
	{
		typedef Resource::GetLocalityGID_action_Resource action_type;
		return action_type()(base_type::get_id());
	}

	// method that returns the number of this resource locality
	hpx::future<unsigned int> GetLocalityID(hpx::launch::async_policy)
	{
		typedef Resource::GetLocalityID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id());
	}

	unsigned int GetLocalityID()
	{
		typedef Resource::GetLocalityID_action_Resource action_type;
		return action_type()(base_type::get_id());	
	}


	/** Static organizer's interface **/
	hpx::future<void> Join(hpx::launch::async_policy, idp_t idp, std::string const& name)
	{
		return hpx::async([&](){
			typedef Closure::Join_action_Closure action_type;
			hpx::async<action_type>(this->get_id(), idp, name).get();

			// this barrier had to be placed here, and not inside the static organizer because it only works outside of components and the component only accepts one action at a time
			// synchronization between the elements of the static_organizer, to ensure the insertion of all resources and the same level of synchronization
			if(_total_members > 1) 
			{
				hpx::lcos::barrier barrier(std::to_string(IdpGlobal()), _total_members, GetIdm(hpx::launch::async, idp).get());
				barrier.wait();
			}
		});	

	}

	void Join(idp_t idp, std::string const& name)
	{
		typedef Closure::Join_action_Closure action_type;
		action_type()(this->get_id(), idp, name);

		// this barrier had to be placed here, and not inside the static organizer because it only works outside of components and the component only accepts one action at a time
		// synchronization between the elements of the static_organizer, to ensure the insertion of all resources and the same level of synchronization
		if(_total_members > 1) 
		{
			hpx::lcos::barrier barrier(std::to_string(IdpGlobal()), _total_members, GetIdm(idp));
			barrier.wait();
		}
	}

	hpx::future<void> Leave(hpx::launch::async_policy, idp_t idp)
	{
		typedef Closure::Leave_action_Closure action_type;
		return hpx::async<action_type>(this->get_id(), idp);
	}

	void Leave(idp_t idp)
	{
		typedef Closure::Leave_action_Closure action_type;
		return action_type()(this->get_id(), idp);
	}

	hpx::future<idp_t> GetParent(hpx::launch::async_policy)
	{
		typedef Closure::GetParent_action_Closure action_type;
		return hpx::async<action_type>(this->get_id());
	}

	idp_t GetParent()
	{
		typedef Closure::GetParent_action_Closure action_type;
		return action_type()(this->get_id());
	}

	hpx::future<std::size_t> GetTotalMembers(hpx::launch::async_policy)
	{
		typedef Closure::GetTotalMembers_action_Closure action_type;
		return hpx::async<action_type>(this->get_id());
	}

	std::size_t GetTotalMembers()
	{
		typedef Closure::GetTotalMembers_action_Closure action_type;
		return action_type()(this->get_id());
	}

	hpx::future<std::size_t> GetFixedTotalMembers(hpx::launch::async_policy)
	{
		typedef Closure::GetFixedTotalMembers_action_Closure action_type;
		return hpx::async<action_type>(this->get_id());
	}

	std::size_t GetFixedTotalMembers()
	{
		typedef Closure::GetFixedTotalMembers_action_Closure action_type;
		return action_type()(this->get_id());
	}

	hpx::future<std::vector<idp_t>> GetMemberList(hpx::launch::async_policy)
	{
		typedef Closure::GetMemberList_action_Closure action_type;
		return hpx::async<action_type>(this->get_id());
	}

	std::vector<idp_t> GetMemberList()
	{
		typedef Closure::GetMemberList_action_Closure action_type;
		return action_type()(this->get_id());
	}

    hpx::future<idp_t> GetIdp(hpx::launch::async_policy, idm_t idm)
	{
		typedef Closure::GetIdp1_action_Closure action_type;
		return hpx::async<action_type>(this->get_id(), idm);
	}

    idp_t GetIdp(idm_t idm)
	{
		typedef Closure::GetIdp1_action_Closure action_type;
		return action_type()(this->get_id(), idm);
	}

    hpx::future<idm_t> GetIdm(hpx::launch::async_policy, idp_t idp)
	{
		typedef Closure::GetIdm1_action_Closure action_type;
		return hpx::async<action_type>(this->get_id(), idp);
	}

    idm_t GetIdm(idp_t idp)
	{
		typedef Closure::GetIdm1_action_Closure action_type;
		return action_type()(this->get_id(), idp);
	}

    hpx::future<idm_t> GetIdm(hpx::launch::async_policy, std::string const& name)
	{
		typedef Closure::GetIdm2_action_Closure action_type;
		return hpx::async<action_type>(this->get_id(), name);
	}

    idm_t GetIdm(std::string const& name)
	{
		typedef Closure::GetIdm2_action_Closure action_type;
		return action_type()(this->get_id(), name);
	}

	hpx::future<idp_t> GetStaticOrganizerIdp(hpx::launch::async_policy)
	{
		typedef Closure::GetStaticOrganizerIdp_action_Closure action_type;
		return hpx::async<action_type>(this->get_id());
	}

	idp_t GetStaticOrganizerIdp()
	{
		typedef Closure::GetStaticOrganizerIdp_action_Closure action_type;
		return action_type()(this->get_id());
	}

	/** Local Client's interface **/
	// local idp of this resource
	hpx::future<idp_t> Idp(hpx::launch::async_policy) {
		return hpx::make_ready_future(_idp);
	}

	idp_t Idp() {
		return _idp;
	}

	// Returns component's GID
	hpx::future<hpx::id_type> GetGid(hpx::launch::async_policy) {
	  	return hpx::make_ready_future(this->get_id());
	}

	hpx::id_type GetGid() {
	  	return this->get_id();
	}

	hpx::future<int> GetComponentType(hpx::launch::async_policy)
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
		return hpx::make_ready_future(3);
	}

	int GetComponentType()
	{
		return 3;
	}

	// For compilation purposes only, it is never used here!
	hpx::future<hpx::id_type> GetMailboxGid(hpx::launch::async_policy) {
		return hpx::make_ready_future(hpx::find_here());
	}

	hpx::id_type GetMailboxGid() {
		return hpx::find_here();
	}


private:
	hpx::future<hpx::id_type> create_server(idp_t idp, unsigned int total_members, idp_t parent) {
		return hpx::local_new<Closure>(idp, total_members, parent);
	}

	hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, unsigned int total_members, idp_t parent) {
		return hpx::new_<Closure>(locality, idp, total_members, parent);
	}

	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
		ar & _total_members;
	}

	idp_t _idp; // local idp
	unsigned int _total_members; // This variable can live here to becouse it's static, here and in the respective component
};


}


#endif