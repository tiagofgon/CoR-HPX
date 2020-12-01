#ifndef COR_OPERON_CLIENT_HPP
#define COR_OPERON_CLIENT_HPP

#include "cor/resources/operon.hpp"

#include <hpx/hpx.hpp>


namespace cor {

class Operon_Client : hpx::components::client_base<Operon_Client, Operon>
{

public:
	typedef hpx::components::client_base<Operon_Client, Operon> base_type;

	friend class hpx::serialization::access;

	typedef nullptr_t organizer;
	
	// Default construct an empty client side representation (not
	// connected to any existing component). Also needed for serialization
	Operon_Client()
	{}

	// Create a client side representation for the existing
	// Operon instance with the given GID
	Operon_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Operon_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Operon_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	// Constructor for replicas
	Operon_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Operon_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Operon_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	// Standard constructor with parameters
	Operon_Client(idp_t idp, std::size_t num_hpx_threads) :
		base_type(create_server(idp, num_hpx_threads)),
		_idp(idp)
	{

	}


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


	/** Executor's interface **/
	void Dispatch(void (*taskfct)(void *), void *arg)
	{
		ensure_ptr();
		// typedef Operon::Dispatch_action_Operon action_type;
		// return hpx::async<action_type>(this->get_id(), f).get();
		//std::shared_ptr<Operon> ptr2 = hpx::get_ptr<Operon>(hpx::launch::sync, this->get_id());
		return ptr->Dispatch(taskfct, arg);
	}

	int GetRank()
	{
		ensure_ptr();
		//std::shared_ptr<Operon> ptr2 = hpx::get_ptr<Operon>(hpx::launch::sync, this->get_id());
		return ptr->GetRank();
	}

	void WaitForIdle()
	{
		ensure_ptr();
		//std::shared_ptr<Operon> ptr2 = hpx::get_ptr<Operon>(hpx::launch::sync, this->get_id());
		return ptr->WaitForIdle();
	}

	int GetNumThreads()
	{
		ensure_ptr();
		//std::shared_ptr<Operon> ptr2 = hpx::get_ptr<Operon>(hpx::launch::sync, this->get_id());
		return ptr->GetNumThreads();
		// typedef Operon::GetNumThreads_action_Operon action_type;
		// return hpx::async<action_type>(this->get_id()).get();
	}

	std::pair<int,int> ScheduleStatic(int Beg, int End)
	{
		ensure_ptr();
		return ptr->ScheduleStatic(Beg, End);
	}

	std::pair<int,int> ScheduleDynamic(int Beg, int End, int chunk)
	{
		ensure_ptr();
		return ptr->ScheduleDynamic(Beg, End, chunk);
	}

	std::pair<int,int> ScheduleGuided(int Beg, int End, int chunk)
	{
		ensure_ptr();
		return ptr->ScheduleGuided(Beg, End, chunk);
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
		return 4;
	}

	// For compilation purposes only, it is never used here!
	hpx::id_type GetMailboxGid() {
		return hpx::find_here();
	}
	
private:
	hpx::future<hpx::id_type> create_server(idp_t idp, std::size_t num_hpx_threads) {
		return hpx::local_new<Operon>(idp, num_hpx_threads);
	}

	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
	}

	idp_t _idp; // local idp

	mutable std::shared_ptr<Operon> ptr;

	void ensure_ptr() const {
		if (!ptr) {
			std::cout << "ensure_ptr()" << std::endl;
			ptr = hpx::get_ptr<Operon>(hpx::launch::sync, this->get_id());
		}
	}




};


}


#endif