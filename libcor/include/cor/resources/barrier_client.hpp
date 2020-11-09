
#ifndef COR_BARRIER_CLIENT_HPP
#define COR_BARRIER_CLIENT_HPP


#include <hpx/hpx.hpp>

#include "cor/resources/barrier.hpp"

#include "cor/system/system.hpp"
#include "cor/system/pod_client.hpp"
#include "cor/resources/closure_client.hpp"


namespace cor {

class Barrier_Client : hpx::components::client_base<Barrier_Client, Barrier>
{

private:
	static hpx::future<hpx::id_type> create_server(idp_t idp, idp_t clos) {
		return hpx::local_new<Barrier>(idp, clos);
	}
	static hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, idp_t clos) {
		return hpx::new_<Barrier>(locality, idp, clos);
	}

public:
	typedef hpx::components::client_base<Barrier_Client, Barrier> base_type;

	friend class hpx::serialization::access;
	friend class Barrier;

	typedef nullptr_t organizer;
	
	/// Default construct an empty client side representation (not
	/// connected to any existing component). Also needed for serialization
	Barrier_Client()
	{}

	/// Create a client side representation for the existing
	/// Barrier instance with the given GID
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

	// Construtor para réplicas
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


	/** Resource interface **/
	// método que retorna o idp global do recurso, que está presente na classe Resource
	idp_t IdpGlobal()
	{
		typedef Resource::Idp_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	hpx::id_type GetLocalityGID()
	{
		typedef Resource::GetLocalityGID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	unsigned int GetLocalityID()
	{
		typedef Resource::GetLocalityID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}


	/** SBarrier interface **/
	void Synchronize()
	{
		typedef Barrier::Synchronize_action_Barrier action_type;
		hpx::async<action_type>(this->get_id()).get();

		auto _clos = GetIdpClos();
		auto sorg = global::pod->GetLocalResource<cor::Closure_Client>(_clos);
		std::size_t total_members = sorg->GetTotalMembers();

		auto active_rsc_idp = global::pod->GetActiveResourceIdp();
		auto idm = sorg->GetIdm(active_rsc_idp);

		// esta barreira teve de ser colocada aqui, e nao dentro do SBarrier porque só funciona fora de components
		// Tem o nome do idp original de component barrier
		hpx::lcos::barrier barrier(std::to_string(IdpGlobal()), total_members, idm);
		barrier.wait();
		return;
	}

	// método para ir buscar o atributo _clos, que é o idp da clausura que esta dentro da classe SBarrier
	idp_t GetIdpClos()
	{
		typedef Barrier::GetIdpClos_action_Barrier action_type;
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
		return 7;
	}

	// Só para fins de compilação, não é usado aqui nunca!
	hpx::id_type GetMailboxGid() {
		return hpx::find_here();
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