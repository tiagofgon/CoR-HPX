#ifndef COR_BARRIER_CLIENT_HPP
#define COR_BARRIER_CLIENT_HPP

#include "cor/resources/barrier.hpp"
#include "cor/system/system.hpp"
#include "cor/system/pod_client.hpp"
#include "cor/resources/closure_client.hpp"

#include <hpx/hpx.hpp>


namespace cor {

class Barrier_Client : hpx::components::client_base<Barrier_Client, Barrier>
{

public:
	typedef hpx::components::client_base<Barrier_Client, Barrier> base_type;

	friend class hpx::serialization::access;

	typedef nullptr_t organizer;
	
	// Default construct an empty client side representation (not
	// connected to any existing component). Also needed for serialization
	Barrier_Client()
	{}

	// Create a client side representation for the existing
	// Barrier instance with the given GID
	Barrier_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Barrier_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Barrier_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	// Constructor for replicas
	Barrier_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Barrier_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Barrier_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	/// Standard constructor with parameters
	Barrier_Client(idp_t idp, idp_t clos) :
		base_type(create_server(idp, clos)),
		_idp(idp)
	{}

	Barrier_Client(idp_t idp, hpx::id_type locality, idp_t clos) :
		base_type(create_server_remote(idp, locality, clos)),
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


	/** SBarrier's interface **/
	void Synchronize()
	{
		typedef Barrier::Synchronize_action_Barrier action_type;
		hpx::async<action_type>(this->get_id()).get();

		auto _clos = GetIdpClos();
		auto sorg = global::pod->GetLocalResource<cor::Closure_Client>(_clos);
		std::size_t total_members = sorg->GetTotalMembers();

		auto active_rsc_idp = global::pod->GetActiveResourceIdp();
		auto idm = sorg->GetIdm(active_rsc_idp);

		// this barrier had to be placed here, and not inside SBarrier component because it only works outside of components
		hpx::lcos::barrier barrier(std::to_string(IdpGlobal()), total_members, idm);
		barrier.wait();
		return;
	}

	// method to get the idp of the this barrier's closure
	idp_t GetIdpClos()
	{
		typedef Barrier::GetIdpClos_action_Barrier action_type;
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
		return 7;
	}

	// For compilation purposes only, it is never used here!
	hpx::id_type GetMailboxGid() {
		return hpx::find_here();
	}
	

private:
	hpx::future<hpx::id_type> create_server(idp_t idp, idp_t clos) {
		return hpx::local_new<Barrier>(idp, clos);
	}

	hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, idp_t clos) {
		return hpx::new_<Barrier>(locality, idp, clos);
	}

	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
	}

	idp_t _idp; // local idp

};


}

#endif