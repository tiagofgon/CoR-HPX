
#ifndef COR_PROTOAGENT_CLIENT_HPP
#define COR_PROTOAGENT_CLIENT_HPP

#include "cor/resources/proto_agent.hpp"

#include <hpx/hpx.hpp>


namespace cor {

template <typename> class ProtoAgent_Client;

template <typename R, typename ... P>
class ProtoAgent_Client<R(P...)> : hpx::components::client_base<ProtoAgent_Client<R(P...)>, ProtoAgent<R(P...)>>
{

private:
	static hpx::future<hpx::id_type> create_server(idp_t idp, std::string const& module, std::string const& function) {
		return hpx::local_new<ProtoAgent<R(P...)>>(idp, module, function);
	}
	static hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, std::string const& module, std::string const& function) {
		return hpx::new_<ProtoAgent<R(P...)>>(locality, idp, module, function);
	}

public:
	typedef hpx::components::client_base<ProtoAgent_Client<R(P...)>, ProtoAgent<R(P...)>> base_type;

	friend class hpx::serialization::access;

	typedef nullptr_t organizer;
	
	/// Default construct an empty client side representation (not
	/// connected to any existing component). Also needed for serialization
	ProtoAgent_Client()
	{}

	/// Create a client side representation for the existing
	/// ProtoAgent instance with the given GID
	ProtoAgent_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	ProtoAgent_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	ProtoAgent_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	// Construtor para réplicas
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

	/// Standard constructor with parameters
	ProtoAgent_Client(idp_t idp, std::string const& module, std::string const& function) :
		base_type(create_server(idp, module, function)),
		_idp(idp)
	{}

	ProtoAgent_Client(idp_t idp, hpx::id_type locality, std::string const& module, std::string const& function) :
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
		typedef typename cor::ProtoAgent<R(P...)>::template Run_action_ProtoAgent<Args...> action_type;
		return hpx::async<action_type>(this->get_id(), std::forward<Args>(args)... ).get();
	}

	template <typename ... Args>
	void Run_here(Args&&... args)
	{
		Migrate(hpx::find_here());
		typedef typename cor::ProtoAgent<R(P...)>::template Run_action_ProtoAgent<Args...> action_type;
		return hpx::async<action_type>(this->get_id(), std::forward<Args>(args)... ).get();
	}

	void Wait()
	{
		typedef typename cor::ProtoAgent<R(P...)>::Wait_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	void Wait_here()
	{
		Migrate(hpx::find_here());
		typedef typename cor::ProtoAgent<R(P...)>::Wait_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	R Get()
	{
		typedef typename cor::ProtoAgent<R(P...)>::Get_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	R Get_here()
	{
		Migrate(hpx::find_here());
		typedef typename cor::ProtoAgent<R(P...)>::Get_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	void ChangeIdp(idp_t idp)
	{
		typedef typename cor::ProtoAgent<R(P...)>::ChangeIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}

	void ChangeIdp_here(idp_t idp)
	{
		Migrate(hpx::find_here());
		typedef typename cor::ProtoAgent<R(P...)>::ChangeIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}

	void ResumeIdp()
	{
		typedef typename cor::ProtoAgent<R(P...)>::ResumeIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	void ResumeIdp_here()
	{
		Migrate(hpx::find_here());
		typedef typename cor::ProtoAgent<R(P...)>::ResumeIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	idp_t CurrentIdp()
	{
		typedef typename cor::ProtoAgent<R(P...)>::CurrentIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	idp_t CurrentIdp_here()
	{
		Migrate(hpx::find_here());
		typedef typename cor::ProtoAgent<R(P...)>::CurrentIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	idp_t OriginalIdp()
	{
		typedef typename cor::ProtoAgent<R(P...)>::OriginalIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}
	
	idp_t OriginalIdp_here()
	{
		Migrate(hpx::find_here());
		typedef typename cor::ProtoAgent<R(P...)>::OriginalIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}
	
	idp_t GetExecutorIdp()
	{
		typedef typename cor::ProtoAgent<R(P...)>::GetExecutorIdp_action_ProtoAgent action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}
	
	idp_t GetExecutorIdp_here()
	{
		Migrate(hpx::find_here());
		typedef typename cor::ProtoAgent<R(P...)>::GetExecutorIdp_action_ProtoAgent action_type;
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
		return 4;
	}

	void Migrate(hpx::id_type dest)
	{
		hpx::components::migrate<ProtoAgent_Client<R(P...)>>(this->get_id(), dest).get();
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