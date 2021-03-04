#ifndef COR_MULTICHANNEL_CLIENT_HPP
#define COR_MULTICHANNEL_CLIENT_HPP

#include "cor/resources/multi_channel.hpp"

#include <hpx/hpx.hpp>



namespace cor {

template <typename T>
class MultiChannel_Client : hpx::components::client_base<MultiChannel_Client<T>, MultiChannel<T>> 
{

public:
	typedef hpx::components::client_base<MultiChannel_Client<T>, MultiChannel<T>> base_type;

	friend class hpx::serialization::access;

	typedef nullptr_t organizer;

	// Default construct an empty client side representation (not
	// connected to any existing component). Also needed for serialization
    MultiChannel_Client()
    {}

	// Create a client side representation for the existing
	// MultiChannel instance with the given GID
    MultiChannel_Client(hpx::future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(IdpGlobal())
    {}

    MultiChannel_Client(hpx::shared_future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(IdpGlobal())
    {}

    MultiChannel_Client(hpx::id_type && id) :
        base_type(std::move(id)),
		_idp(IdpGlobal())
    {}

	// Constructor for replicas
    MultiChannel_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(idp)
    {}

    MultiChannel_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(idp)
    {}

    MultiChannel_Client(idp_t idp, hpx::id_type && id) :
        base_type(std::move(id)),
		_idp(idp)
    {}

	// Standard constructor with parameters
	template <typename ... Args>
    MultiChannel_Client(idp_t idp, std::string const& myself, Args ... args) :
        base_type(create_server(idp, myself, args...)),
		_idp(idp)
    {}

	template <typename ... Args>
    MultiChannel_Client(idp_t idp, hpx::id_type locality, std::string const& myself, Args ... args) :
        base_type(create_server_remote(idp, locality, myself, args...)),
		_idp(idp)
    {}

	template <typename ... Args>
    MultiChannel_Client(idp_t idp, unsigned int pod_id, std::string const& myself, Args ... args) :
        base_type(create_server(idp, myself, args...)),
		_idp(idp)
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




	/** MultiChannel's interface **/
    hpx::future<T> Get(std::string const& partner, std::size_t step) 
	{
		typedef typename MultiChannel<T>::Get_action_MultiChannel action_type;
		return action_type()(this->get_id(), partner, step); 	
	}

    void Set(T&& t, std::string const& partner, std::size_t step) 
	{
		typedef typename MultiChannel<T>::Set_action_MultiChannel action_type;
		return action_type()(this->get_id(), std::move(t), partner, step); 	
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
		6 - MultiChannel
		7 - Barrier
		8 - Mutex
		9 - RWMutex
		10 - MultiChannel
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
	template <typename ... Args>
	hpx::future<hpx::id_type> create_server(idp_t idp, std::string const& myself, Args ... args) {
		return hpx::local_new<MultiChannel<T>>(idp, myself, args...);
	}

	template <typename ... Args>
	hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, std::string const& myself, Args ... args) {
		return hpx::new_<MultiChannel<T>>(locality, idp, myself, args...);
	}

	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
	}

	idp_t _idp;

};

	
}


#endif
