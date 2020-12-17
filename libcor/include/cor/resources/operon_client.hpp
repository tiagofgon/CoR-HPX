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

	typedef Mailbox organizer;
	
	// Default construct an empty client side representation (not
	// connected to any existing component). Also needed for serialization
	Operon_Client()
	{}

	// Create a client side representation for the existing
	// Operon instance with the given GID
	Operon_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal().get())
	{}

	Operon_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal().get())
	{}

	Operon_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal().get())
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


	/** Executor's interface **/
	hpx::future<int> GetRank()
	{
		ensure_ptr();
		//std::shared_ptr<Operon> ptr2 = hpx::get_ptr<Operon>(hpx::launch::sync, this->get_id());
		return hpx::make_ready_future(ptr->GetRank());
	}

	hpx::future<int> GetNumThreads()
	{
		ensure_ptr();
		//std::shared_ptr<Operon> ptr2 = hpx::get_ptr<Operon>(hpx::launch::sync, this->get_id());
		return hpx::make_ready_future(ptr->GetNumThreads());
		// typedef Operon::GetNumThreads_action_Operon action_type;
		// return hpx::async<action_type>(this->get_id());
	}

	hpx::future<std::pair<int,int>> ScheduleStatic(int Beg, int End)
	{
		ensure_ptr();
		return hpx::make_ready_future(ptr->ScheduleStatic(Beg, End));
	}

	hpx::future<std::vector<std::pair<int,int>>> ScheduleStatic(int Beg, int End, int chunk)
	{
		ensure_ptr();
		return hpx::make_ready_future(ptr->ScheduleStatic(Beg, End, chunk));
	}

	hpx::future<std::pair<int,int>> ScheduleDynamic(int Beg, int End, int chunk)
	{
		ensure_ptr();
		return hpx::make_ready_future(ptr->ScheduleDynamic(Beg, End, chunk));
	}

	hpx::future<std::pair<int,int>> ScheduleGuided(int Beg, int End, int chunk)
	{
		ensure_ptr();
		return hpx::make_ready_future(ptr->ScheduleGuided(Beg, End, chunk));
	}

	template < typename ... Args >
    hpx::future<void> Dispatch(hpx::function<void(Args...)> func, Args ... args) {
		typedef Operon::Dispatch_action_Operon<Args...> action_type;
		return hpx::async<action_type>(this->get_id(), func, args...);
    }

    hpx::future<void> Dispatch(hpx::function<void()> func) {
		typedef Operon::Dispatch_void_action_Operon action_type;
		return hpx::async<action_type>(this->get_id(), func);
    }


    /* Mailbox's interface */
    hpx::future<void> Send(idp_t dest, Message const& msg)
	{
		typedef typename cor::Operon::Send1_action_Operon action_type;
		return hpx::async<action_type>(this->get_id(), dest, msg);
	}

    hpx::future<void> Send(std::vector<idp_t> const& dests, Message const& msg)
	{
		typedef typename cor::Operon::Send2_action_Operon action_type;
		return hpx::async<action_type>(this->get_id(), dests, msg);
	}

    hpx::future<Message> Receive()
	{
		typedef typename cor::Operon::Receive1_action_Operon action_type;
		return hpx::async<action_type>(this->get_id());
	}

    hpx::future<Message> Receive(idp_t source)
	{
		typedef typename cor::Operon::Receive2_action_Operon action_type;
		return hpx::async<action_type>(this->get_id(), source);
	}

    hpx::future<void> Broadcast(idp_t clos, Message const& msg)
	{
		typedef typename cor::Operon::Broadcast_action_Operon action_type;
		return hpx::async<action_type>(this->get_id(), clos, msg);
	}

    hpx::future<void> Send(idm_t rank, idp_t clos, Message const& msg)
	{
		typedef typename cor::Operon::Send3_action_Operon action_type;
		return hpx::async<action_type>(this->get_id(), rank, clos, msg);
	}

    hpx::future<Message> Receive(idm_t rank, idp_t clos)
	{
		typedef typename cor::Operon::Receive3_action_Operon action_type;
		return hpx::async<action_type>(this->get_id(), rank, clos);
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
		10 - Operon
		*/
		return hpx::make_ready_future(10);
	}

	// For compilation purposes only, it is never used here!
	hpx::future<hpx::id_type> GetMailboxGid() {
		return hpx::make_ready_future(hpx::find_here());
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
			//std::cout << "ensure_ptr()" << std::endl;
			ptr = hpx::get_ptr<Operon>(hpx::launch::sync, this->get_id());
		}
	}




};


}


#endif