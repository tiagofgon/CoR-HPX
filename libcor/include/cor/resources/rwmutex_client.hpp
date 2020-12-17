#ifndef COR_RWMUTEX_CLIENT_HPP
#define COR_RWMUTEX_CLIENT_HPP

#include "cor/resources/rwmutex.hpp"

#include <hpx/hpx.hpp>


namespace cor {

struct RWMutex_Client: hpx::components::client_base<RWMutex_Client, RWMutex>
{

public:
	typedef hpx::components::client_base<RWMutex_Client, RWMutex> base_type;

	friend class hpx::serialization::access;

	typedef nullptr_t organizer;

	// Default construct an empty client side representation (not
	// connected to any existing component. Also needed for serialization
	RWMutex_Client()
	{}

	// Create a client side representation for the existing
	// Closure_Component instance with the given GID
	RWMutex_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal().get())
	{}

	RWMutex_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal().get())
	{}

	RWMutex_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal().get())
	{}

	// Constructor for replicas
	RWMutex_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	RWMutex_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	RWMutex_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	/// Standard constructor with parameters
	RWMutex_Client(idp_t idp) :
		base_type(create_server(idp)),
		_idp(idp)
	{}

	RWMutex_Client(idp_t idp, hpx::id_type locality) :
		base_type(create_server_remote(idp, locality)),
		_idp(idp)
	{}


	/** Resource's interface **/
	// method that returns the global idp of the resource, which is present in the class Resource
	hpx::future<idp_t> IdpGlobal()
	{
	  typedef Resource::Idp_action_Resource action_type;
	  return hpx::async<action_type>(base_type::get_id());
	}

	// method that returns the GID(hpx::id_type) of this resource locality
	hpx::future<hpx::id_type> GetLocalityGID()
	{
		typedef Resource::GetLocalityGID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id());
	}

	// method that returns the number of this resource locality
	hpx::future<unsigned int> GetLocalityID()
	{
		typedef Resource::GetLocalityID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id());
	}


	/** RWMutex's interface **/
    hpx::future<void> AcquireRead()
	{
	  typedef RWMutex::AcquireRead_action_RWMutex action_type;
	  return hpx::async<action_type>(this->get_id());
	}

    hpx::future<void> ReleaseRead()
	{
	  typedef RWMutex::ReleaseRead_action_RWMutex action_type;
	  return hpx::async<action_type>(this->get_id());
	}

    hpx::future<void> AcquireWrite()
	{
	  typedef RWMutex::AcquireWrite_action_RWMutex action_type;
	  return hpx::async<action_type>(this->get_id());
	}

    hpx::future<void> ReleaseWrite()
	{
	  typedef RWMutex::ReleaseWrite_action_RWMutex action_type;
	  return hpx::async<action_type>(this->get_id());
	}


	/** Local Client's interface **/
	// local idp of this resource
	hpx::future<idp_t> Idp() {
		return hpx::make_ready_future(_idp);
	}

	// Returns component's GID
	hpx::future<hpx::id_type> GetGid() {
	  return hpx::make_ready_future(this->get_id());
	}

	hpx::future<int> GetComponentType()
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
		return hpx::make_ready_future(9);
	}

	// For compilation purposes only, it is never used here!
	hpx::future<hpx::id_type> GetMailboxGid() {
		return hpx::make_ready_future(hpx::find_here());
	}


  	private:
	hpx::future<hpx::id_type> create_server(idp_t idp) {
		return hpx::local_new<RWMutex>(idp);
	}

	hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality) {
		return hpx::new_<RWMutex>(locality, idp);
	}

	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
	}

	idp_t _idp; // local idp

};


}

#endif