#ifndef COR_PROTOAGENT_CLIENT_HPP
#define COR_PROTOAGENT_CLIENT_HPP

#include "cor/resources/proto_agent.hpp"

#include <hpx/hpx.hpp>


namespace cor {

template <typename> class ProtoAgent_Client;

template <typename R, typename ... P>
class ProtoAgent_Client<R(P...)> : hpx::components::client_base<ProtoAgent_Client<R(P...)>, ProtoAgent<R(P...)>>
{

public:
	typedef hpx::components::client_base<ProtoAgent_Client<R(P...)>, ProtoAgent<R(P...)>> base_type;

	friend class hpx::serialization::access;

	typedef nullptr_t organizer;
	
	// Default construct an empty client side representation (not
	// connected to any existing component). Also needed for serialization
	ProtoAgent_Client()
	{}

	// Create a client side representation for the existing
	// ProtoAgent instance with the given GID
	ProtoAgent_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal().get())
	{}

	ProtoAgent_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal().get())
	{}

	ProtoAgent_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal().get())
	{}

	// Constructor for replicas
	ProtoAgent_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	ProtoAgent_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	ProtoAgent_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	// Standard constructor with parameters
	ProtoAgent_Client(idp_t idp, std::function<R(P...)> const& f) :
		base_type(create_server(idp, f)),
		_idp(idp)
	{}

	ProtoAgent_Client(idp_t idp, std::string const& module, std::string const& function) :
		base_type(create_server(idp, module, function)),
		_idp(idp)
	{}

	ProtoAgent_Client(idp_t idp, hpx::id_type locality, std::function<R(P...)> const& f) :
		base_type(create_server_remote(idp, locality, f)),
		_idp(idp)
	{}

	ProtoAgent_Client(idp_t idp, hpx::id_type locality, std::string const& module, std::string const& function) :
		base_type(create_server_remote(idp, locality, module, function)),
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


	/** Executor's interface **/
	hpx::future<void> Run_void(std::shared_ptr<void> arg)
	{
		typedef typename cor::ProtoAgent<R(P...)>::Run_void_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id(), arg);
	}

	template <typename ... Args>
	hpx::future<void> Run(Args&&... args)
	{
		typedef typename cor::ProtoAgent<R(P...)>::template Run_action_ProtoAgent<Args...> action_type;
		return hpx::async<action_type>(this->get_id(), std::forward<Args>(args)... );
	}

	hpx::future<void> Wait()
	{
		typedef typename cor::ProtoAgent<R(P...)>::Wait_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id());
	}

	hpx::future<R> Get()
	{
		typedef typename cor::ProtoAgent<R(P...)>::Get_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id());
	}

	hpx::future<void> ChangeIdp(idp_t idp)
	{
		typedef typename cor::ProtoAgent<R(P...)>::ChangeIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id(), idp);
	}

	hpx::future<void> ResumeIdp()
	{
		typedef typename cor::ProtoAgent<R(P...)>::ResumeIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id());
	}

	hpx::future<idp_t> CurrentIdp()
	{
		typedef typename cor::ProtoAgent<R(P...)>::CurrentIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id());
	}

	hpx::future<idp_t> OriginalIdp()
	{
		typedef typename cor::ProtoAgent<R(P...)>::OriginalIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id());
	}

	hpx::future<idp_t> GetExecutorIdp()
	{
		typedef typename cor::ProtoAgent<R(P...)>::GetExecutorIdp_action_ProtoAgent action_type;
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
		return hpx::make_ready_future(4);
	}

	// For compilation purposes only, it is never used here!
	hpx::future<hpx::id_type> GetMailboxGid() {
		return hpx::make_ready_future(hpx::find_here());
	}
	
private:
	hpx::future<hpx::id_type> create_server(idp_t idp, std::function<R(P...)> const& f) {
		return hpx::local_new<ProtoAgent<R(P...)>>(idp, f);
	}

	hpx::future<hpx::id_type> create_server(idp_t idp, std::string const& module, std::string const& function) {
		return hpx::local_new<ProtoAgent<R(P...)>>(idp, module, function);
	}

	hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, std::function<R(P...)> const& f) {
		return hpx::new_<ProtoAgent<R(P...)>>(locality, idp, f);
	}

	hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, std::string const& module, std::string const& function) {
		return hpx::new_<ProtoAgent<R(P...)>>(locality, idp, module, function);
	}

	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
	}

	idp_t _idp; // local idp

};


}


#endif