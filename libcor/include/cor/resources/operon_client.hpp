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




	/** Executor's interface **/
	hpx::future<int> GetRank(hpx::launch::async_policy)
	{
		ensure_ptr();
		return hpx::async([&](){
			return ptr->GetRank();
		});	
	}

	int GetRank()
	{
		ensure_ptr();
		return ptr->GetRank();
	}

	hpx::future<int> GetNumThreads(hpx::launch::async_policy)
	{
		typedef Operon::GetNumThreads_action_Operon action_type;
		return hpx::async<action_type>(this->get_id());
	}

	int GetNumThreads()
	{
		typedef Operon::GetNumThreads_action_Operon action_type;
		return action_type()(this->get_id());
	}

	hpx::future<std::pair<int,int>> ScheduleStatic(hpx::launch::async_policy, int Beg, int End)
	{
		ensure_ptr();
		return hpx::async([&](){
			return ptr->ScheduleStatic(Beg, End);
		});	
	}

	std::pair<int,int> ScheduleStatic(int Beg, int End)
	{
		ensure_ptr();
		return ptr->ScheduleStatic(Beg, End);
	}

	hpx::future<std::vector<std::pair<int,int>>> ScheduleStatic(hpx::launch::async_policy, int Beg, int End, int chunk)
	{
		ensure_ptr();
		return hpx::async([&](){
			return ptr->ScheduleStatic(Beg, End, chunk);
		});	
	}

	std::vector<std::pair<int,int>> ScheduleStatic(int Beg, int End, int chunk)
	{
		ensure_ptr();
		return ptr->ScheduleStatic(Beg, End, chunk);
	}

	hpx::future<std::pair<int,int>> ScheduleDynamic(hpx::launch::async_policy, int Beg, int End, int chunk)
	{
		ensure_ptr();
		return hpx::async([&](){
			return ptr->ScheduleDynamic(Beg, End, chunk);
		});	
	}
	
	std::pair<int,int> ScheduleDynamic(int Beg, int End, int chunk)
	{
		ensure_ptr();
		return ptr->ScheduleDynamic(Beg, End, chunk);
	}

	hpx::future<std::pair<int,int>> ScheduleGuided(hpx::launch::async_policy, int Beg, int End, int chunk)
	{
		ensure_ptr();
		return hpx::async([&](){
			return ptr->ScheduleGuided(Beg, End, chunk);
		});	
	}

	std::pair<int,int> ScheduleGuided(int Beg, int End, int chunk)
	{
		ensure_ptr();
		return ptr->ScheduleGuided(Beg, End, chunk);
	}

	template < typename ... Args >
    hpx::future<void> Dispatch(hpx::launch::async_policy, hpx::function<void(Args...)> func, Args ... args) {
		typedef Operon::Dispatch_action_Operon<Args...> action_type;
		return hpx::async<action_type>(this->get_id(), func, std::forward<Args>(args)...);
    }

	template < typename Func, typename ... Args >
    hpx::future<void> Dispatch(Func&& func, Args&&... args) {
		ensure_ptr();
		return hpx::async([&](){
			return ptr->Dispatch(std::forward<Func>(func), std::forward<Args>(args)...);
		});	
    }

    hpx::future<void> Dispatch(hpx::launch::async_policy, hpx::function<void()> func) {
		typedef Operon::Dispatch_void_action_Operon action_type;
		return hpx::async<action_type>(this->get_id(), func);
    }

	template < typename Func>
    hpx::future<void> Dispatch(Func&& func) {
		ensure_ptr();
		return hpx::async([&](){
			return ptr->Dispatch(std::forward<Func>(func));
		});	
    }

	void Wait()
	{
		ensure_ptr();
		return ptr->Wait();
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
		10 - Operon
		11 - UniChannel
		12 - MultiChannel
		*/
		return hpx::make_ready_future(10);
	}

	int GetComponentType()
	{
		return 10;
	}

	// For compilation purposes only, it is never used here!
	hpx::future<hpx::id_type> GetMailboxGid(hpx::launch::async_policy) {
		return hpx::make_ready_future(hpx::find_here());
	}

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
			//std::cout << "ensure_ptr()" << std::endl;
			ptr = hpx::get_ptr<Operon>(hpx::launch::sync, this->get_id());
		}
	}




};


}


#endif