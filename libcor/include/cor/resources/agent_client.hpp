
#ifndef COR_AGENT_CLIENT_HPP
#define COR_AGENT_CLIENT_HPP

#include "cor/resources/agent.hpp"

#include <hpx/hpx.hpp>


namespace cor {

template <typename> class Agent_Client;

template <typename R, typename ... P>
class Agent_Client<R(P...)> : hpx::components::client_base<Agent_Client<R(P...)>, Agent<R(P...)>>
{

private:
	static hpx::future<hpx::id_type> create_server(idp_t idp, std::string const& module, std::string const& function) {
		return hpx::local_new<Agent<R(P...)>>(idp, module, function);
	}
	static hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, std::string const& module, std::string const& function) {
		return hpx::new_<Agent<R(P...)>>(locality, idp, module, function);
	}

public:
	typedef hpx::components::client_base<Agent_Client<R(P...)>, Agent<R(P...)>> base_type;

	friend class hpx::serialization::access;

	typedef Mailbox organizer;
	
	/// Default construct an empty client side representation (not
	/// connected to any existing component). Also needed for serialization
	Agent_Client()
	{}

	/// Create a client side representation for the existing
	/// Agent instance with the given GID
	Agent_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Agent_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Agent_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	// Construtor para réplicas
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
	Agent_Client(idp_t idp, std::string const& module, std::string const& function) :
		base_type(create_server(idp, module, function)),
		_idp(idp)
	{}

	Agent_Client(idp_t idp, hpx::id_type locality, std::string const& module, std::string const& function) :
		base_type(create_server_remote(idp, locality, module, function)),
		_idp(idp)
	{}


	/** Resource interface **/
	// método que retorna o idp global do recurso, que está presente na classe Resource
	idp_t IdpGlobal()
	{
		typedef Resource::Idp_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	idp_t IdpGlobal_here()
	{
		Migrate(hpx::find_here());
		typedef Resource::Idp_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	hpx::id_type GetLocalityGID()
	{
		typedef Resource::GetLocalityGID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}
	
	hpx::id_type GetLocalityGID_here()
	{
		Migrate(hpx::find_here());
		typedef Resource::GetLocalityGID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	unsigned int GetLocalityID()
	{
		typedef Resource::GetLocalityID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	unsigned int GetLocalityID_here()
	{
		Migrate(hpx::find_here());
		typedef Resource::GetLocalityID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}
	
	/** Executor interface **/
	template <typename ... Args>
	void Run(Args&&... args)
	{
		typedef typename cor::Agent<R(P...)>::template Run_action_Agent<Args...> action_type;
		return hpx::async<action_type>(this->get_id(), std::forward<Args>(args)... ).get();
	}

	template <typename ... Args>
	void Run_here(Args&&... args)
	{
		Migrate(hpx::find_here());
		typedef typename cor::Agent<R(P...)>::template Run_action_Agent<Args...> action_type;
		return hpx::async<action_type>(this->get_id(), std::forward<Args>(args)... ).get();
	}

	void Wait()
	{
		typedef typename cor::Agent<R(P...)>::Wait_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	void Wait_here()
	{
		Migrate(hpx::find_here());
		typedef typename cor::Agent<R(P...)>::Wait_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	R Get()
	{
		typedef typename cor::Agent<R(P...)>::Get_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	R Get_here()
	{
		Migrate(hpx::find_here());
		typedef typename cor::Agent<R(P...)>::Get_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	void ChangeIdp(idp_t idp)
	{
		typedef typename cor::Agent<R(P...)>::ChangeIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}

	void ChangeIdp_here(idp_t idp)
	{
		Migrate(hpx::find_here());
		typedef typename cor::Agent<R(P...)>::ChangeIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}

	void ResumeIdp()
	{
		typedef typename cor::Agent<R(P...)>::ResumeIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	void ResumeIdp_here()
	{
		Migrate(hpx::find_here());
		typedef typename cor::Agent<R(P...)>::ResumeIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	idp_t CurrentIdp()
	{
		typedef typename cor::Agent<R(P...)>::CurrentIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	idp_t CurrentIdp_here()
	{
		Migrate(hpx::find_here());
		typedef typename cor::Agent<R(P...)>::CurrentIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	idp_t OriginalIdp()
	{
		typedef typename cor::Agent<R(P...)>::OriginalIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}
	
	idp_t OriginalIdp_here()
	{
		Migrate(hpx::find_here());
		typedef typename cor::Agent<R(P...)>::OriginalIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}
	
	idp_t GetExecutorIdp()
	{
		typedef typename cor::Agent<R(P...)>::GetExecutorIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}
	
	idp_t GetExecutorIdp_here()
	{
		Migrate(hpx::find_here());
		typedef typename cor::Agent<R(P...)>::GetExecutorIdp_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}



    /* Mailbox interface */
    void Send(idp_t dest, Message const& msg)
	{
		typedef typename cor::Agent<R(P...)>::Send1_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), dest, msg).get();
	}

    void Send(std::vector<idp_t> const& dests, Message const& msg)
	{
		typedef typename cor::Agent<R(P...)>::Send2_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), dests, msg).get();
	}

    Message Receive()
	{
		typedef typename cor::Agent<R(P...)>::Receive1_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

    Message Receive(idp_t source)
	{
		typedef typename cor::Agent<R(P...)>::Receive2_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), source).get();
	}

    void Broadcast(idp_t clos, Message const& msg)
	{
		typedef typename cor::Agent<R(P...)>::Broadcast_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), clos, msg).get();
	}

    void Send(idm_t rank, idp_t clos, Message const& msg)
	{
		typedef typename cor::Agent<R(P...)>::Send3_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), rank, clos, msg).get();
	}

    Message Receive(idm_t rank, idp_t clos)
	{
		typedef typename cor::Agent<R(P...)>::Receive3_action_Agent action_type;
		return hpx::async<action_type>(this->get_id(), rank, clos).get();
	}

	// Returns mailbox's GID
	hpx::id_type GetMailboxGid() {
		typedef typename cor::Agent<R(P...)>::GetMailboxGid_action_Agent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	/** Local interface **/
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
		*/
		return 5;
	}

	void Migrate(hpx::id_type dest)
	{
		hpx::components::migrate<Agent_Client<R(P...)>>(this->get_id(), dest).get();
	}

private:
	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
		// std::cout << "serialized\n";
	}

	idp_t _idp; // local idp

};


}

#endif