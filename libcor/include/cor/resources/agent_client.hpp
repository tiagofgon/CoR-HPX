#ifndef COR_AGENT_CLIENT_HPP
#define COR_AGENT_CLIENT_HPP

#include "cor/resources/agent.hpp"

#include <hpx/hpx.hpp>


namespace cor {

template <typename> class Agent_Client;

template <typename R, typename ... P>
class Agent_Client<R(P...)> : hpx::components::client_base<Agent_Client<R(P...)>, Agent<R(P...)>>
{

public:
	typedef hpx::components::client_base<Agent_Client<R(P...)>, Agent<R(P...)>> base_type;

	friend class hpx::serialization::access;

	typedef Mailbox organizer;
	
	// Default construct an empty client side representation (not
	// connected to any existing component). Also needed for serialization
	Agent_Client()
	{}

	// Create a client side representation for the existing
	// Agent instance with the given GID
	Agent_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal().get())
	{}

	Agent_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal().get())
	{}

	Agent_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal().get())
	{}

	// Constructor for replicas
	Agent_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Agent_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Agent_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	/// Standard constructor with parameters
	Agent_Client(idp_t idp, hpx::function<R(P...)> const& f) :
		base_type(create_server(idp, f)),
		_idp(idp)
	{
		// std::cout << "Criado um componente \"Agent_Client\", com idp: " << idp << std::endl;
	}

	Agent_Client(idp_t idp, hpx::id_type locality, hpx::function<R(P...)> const& f) :
		base_type(create_server_remote(idp, locality, f)),
		_idp(idp)
	{}

	Agent_Client(idp_t idp, std::string const& module, std::string const& function) :
		base_type(create_server(idp, module, function)),
		_idp(idp)
	{}

	Agent_Client(idp_t idp, hpx::id_type locality, std::string const& module, std::string const& function) :
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
	template <typename ... Args>
	hpx::future<void> Run(Args&&... args)
	{
		typedef typename cor::Agent<R(P...)>::template Run_action_Agent<Args...> action_type;
		return hpx::async<action_type>(this->get_id(), std::forward<Args>(args)... );
	}

	template <typename ... Args>
	hpx::future<R> RunNow(Args&&... args)
	{
		typedef typename cor::Agent<R(P...)>::template RunNow_action_Agent<Args...> action_type;
		return hpx::async<action_type>(this->get_id(), std::forward<Args>(args)... );
	}

	hpx::future<void> Wait()
	{
		typedef typename cor::Agent<R(P...)>::Wait_action_Agent action_type;
		return hpx::async<action_type>(this->get_id());
	}

	hpx::future<R> Get()
	{
		typedef typename cor::Agent<R(P...)>::Get_action_Agent action_type;
		return hpx::async<action_type>(this->get_id());
	}

	hpx::future<void> ChangeIdp(idp_t idp)
	{
		typedef typename cor::Agent<R(P...)>::ChangeIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), idp);
	}

	hpx::future<void> ResumeIdp()
	{
		typedef typename cor::Agent<R(P...)>::ResumeIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id());
	}

	hpx::future<idp_t> CurrentIdp()
	{
		typedef typename cor::Agent<R(P...)>::CurrentIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id());
	}

	hpx::future<idp_t> OriginalIdp()
	{
		typedef typename cor::Agent<R(P...)>::OriginalIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id());
	}

	hpx::future<idp_t> GetExecutorIdp()
	{
		typedef typename cor::Agent<R(P...)>::GetExecutorIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id());
	}


    /* Mailbox's interface */
    hpx::future<void> Send(idp_t dest, Message const& msg)
	{
		typedef typename cor::Agent<R(P...)>::Send1_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), dest, msg);
	}

    hpx::future<void> Send(std::vector<idp_t> const& dests, Message const& msg)
	{
		typedef typename cor::Agent<R(P...)>::Send2_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), dests, msg);
	}

    hpx::future<Message> Receive()
	{
		typedef typename cor::Agent<R(P...)>::Receive1_action_Agent action_type;
		return hpx::async<action_type>(this->get_id());
	}

    hpx::future<Message> Receive(idp_t source)
	{
		typedef typename cor::Agent<R(P...)>::Receive2_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), source);
	}

    hpx::future<void> Broadcast(idp_t clos, Message const& msg)
	{
		typedef typename cor::Agent<R(P...)>::Broadcast_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), clos, msg);
	}

    hpx::future<void> Send(idm_t rank, idp_t clos, Message const& msg)
	{
		typedef typename cor::Agent<R(P...)>::Send3_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), rank, clos, msg);
	}

    hpx::future<Message> Receive(idm_t rank, idp_t clos)
	{
		typedef typename cor::Agent<R(P...)>::Receive3_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), rank, clos);
	}


	// Returns mailbox's GID
	hpx::future<hpx::id_type> GetMailboxGid() {
		typedef typename cor::Agent<R(P...)>::GetMailboxGid_action_Agent action_type;
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
		return hpx::make_ready_future(5);
	}


private:
	hpx::future<hpx::id_type> create_server(idp_t idp, hpx::function<R(P...)> const& f) {
		return hpx::local_new<Agent<R(P...)>>(idp, f);
	}

	hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, hpx::function<R(P...)> const& f) {
		auto res = hpx::new_<Agent<R(P...)>>(locality, idp, f);
		return res;
	}

	hpx::future<hpx::id_type> create_server(idp_t idp, std::string const& module, std::string const& function) {
		return hpx::local_new<Agent<R(P...)>>(idp, module, function);
	}

	hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, std::string const& module, std::string const& function) {
		auto res = hpx::new_<Agent<R(P...)>>(locality, idp, module, function);
		return res;
	}

	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
	}

	idp_t _idp; // local idp

};


}


#endif