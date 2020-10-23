
#ifndef COR_CLOSURE_CLIENT_HPP
#define COR_CLOSURE_CLIENT_HPP


#include <hpx/hpx.hpp>
#include "cor/resources/closure.hpp"
// #include "cor/system/system.hpp"
// #include "cor/system/pod_client.hpp"
// #include "cor/resources/domain_client.hpp"


namespace cor {

class Domain_Client;
class Pod_Client;

class Closure_Client : hpx::components::client_base<Closure_Client, Closure>
{

private:
	static hpx::future<hpx::id_type> create_server(idp_t idp, unsigned int total_members, idp_t parent) {
		return hpx::local_new<Closure>(idp, total_members, parent);
	}
	static hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, unsigned int total_members, idp_t parent) {
		return hpx::new_<Closure>(locality, idp, total_members, parent);
	}

public:
	typedef hpx::components::client_base<Closure_Client, Closure> base_type;

	friend class hpx::serialization::access;
	friend class Closure;

	typedef StaticOrganizer organizer;

	/// Default construct an empty client side representation (not
	/// connected to any existing component). Also needed for serialization
	Closure_Client()
	{}

	/// Create a client side representation for the existing
	/// Closure instance with the given GID
	Closure_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal()),
		_total_members(GetFixedTotalMembers())
	{}

	Closure_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal()),
		_total_members(GetFixedTotalMembers())
	{}

	Closure_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal()),
		_total_members(GetFixedTotalMembers())
	{}

	// Construtor para réplicas
	Closure_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp),
		_total_members(GetFixedTotalMembers())
	{}

	Closure_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp),
		_total_members(GetFixedTotalMembers())
	{}

	Closure_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp),
		_total_members(GetFixedTotalMembers())
	{}

	/// Standard constructor with parameters
	Closure_Client(idp_t idp, unsigned int total_members, idp_t parent) :
		base_type(create_server(idp, total_members, parent)),
		_idp(idp),
		_total_members(total_members)
	{}

	Closure_Client(idp_t idp, hpx::id_type locality, unsigned int total_members, idp_t parent) :
		base_type(create_server_remote(idp, locality, total_members, parent)),
		_idp(idp),
		_total_members(total_members)
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

	/** Static organizer interface **/
	void Join(idp_t idp, std::string const& name)
	{
		typedef Closure::Join_action_Closure action_type;
		hpx::async<action_type>(this->get_id(), idp, name).get();

		// esta barreira teve de ser colocada aqui, e nao dentro do static organizer porque só funciona fora de components e a component só aceita uma action de cada vez
		// sincronização entre os elementos do static_organizer, para garantir a inserção de todos os recursos e o mesmo nivel de paralelização
		hpx::lcos::barrier barrier(std::to_string(IdpGlobal()), _total_members);
		barrier.wait();
		return;
	}

	void Join_here(idp_t idp, std::string const& name)
	{
		Migrate(hpx::find_here());
		typedef Closure::Join_action_Closure action_type;
		hpx::async<action_type>(this->get_id(), idp, name).get();

		// esta barreira teve de ser colocada aqui, e nao dentro do static organizer porque só funciona fora de components e a component só aceita uma action de cada vez
		// sincronização entre os elementos do static_organizer, para garantir a inserção de todos os recursos e o mesmo nivel de paralelização
		hpx::lcos::barrier barrier(std::to_string(IdpGlobal()), _total_members);
		barrier.wait();
		return;
	}

	void Leave(idp_t idp)
	{
		typedef Closure::Leave_action_Closure action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}

	void Leave_here(idp_t idp)
	{
		Migrate(hpx::find_here());
		typedef Closure::Leave_action_Closure action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}

	idp_t GetParent()
	{
		typedef Closure::GetParent_action_Closure action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	idp_t GetParent_here()
	{
		Migrate(hpx::find_here());
		typedef Closure::GetParent_action_Closure action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	std::size_t GetTotalMembers()
	{
		typedef Closure::GetTotalMembers_action_Closure action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	std::size_t GetTotalMembers_here()
	{
		Migrate(hpx::find_here());
		typedef Closure::GetTotalMembers_action_Closure action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	std::size_t GetFixedTotalMembers()
	{
		typedef Closure::GetFixedTotalMembers_action_Closure action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	std::size_t GetFixedTotalMembers_here()
	{
		Migrate(hpx::find_here());
		typedef Closure::GetFixedTotalMembers_action_Closure action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	std::vector<idp_t> GetMemberList()
	{
		typedef Closure::GetMemberList_action_Closure action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	std::vector<idp_t> GetMemberList_here()
	{
		Migrate(hpx::find_here());
		typedef Closure::GetMemberList_action_Closure action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

    idp_t GetIdp(idm_t idm)
	{
		typedef Closure::GetIdp1_action_Closure action_type;
		return hpx::async<action_type>(this->get_id(), idm).get();
	}

    idp_t GetIdp_here(idm_t idm)
	{
		Migrate(hpx::find_here());
		typedef Closure::GetIdp1_action_Closure action_type;
		return hpx::async<action_type>(this->get_id(), idm).get();
	}

    idm_t GetIdm(idp_t idp)
	{
		typedef Closure::GetIdm1_action_Closure action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}

    idm_t GetIdm_here(idp_t idp)
	{
		Migrate(hpx::find_here());
		typedef Closure::GetIdm1_action_Closure action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}

    idm_t GetIdm(std::string const& name)
	{
		typedef Closure::GetIdm2_action_Closure action_type;
		return hpx::async<action_type>(this->get_id(), name).get();
	}

    idm_t GetIdm_here(std::string const& name)
	{
		Migrate(hpx::find_here());
		typedef Closure::GetIdm2_action_Closure action_type;
		return hpx::async<action_type>(this->get_id(), name).get();
	}

	idp_t GetStaticIdp()
	{
		typedef Closure::GetStaticIdp_action_Closure action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	idp_t GetStaticIdp_here()
	{
		Migrate(hpx::find_here());
		typedef Closure::GetStaticIdp_action_Closure action_type;
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
		return 3;
	}

	void Migrate(hpx::id_type dest)
	{
		hpx::components::migrate<Closure>(this->get_id(), dest).get();
	}
	

private:
	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
		ar & _total_members;
		// std::cout << "serialized\n";
	}

	idp_t _idp; // local idp
	unsigned int _total_members; // This variable can live here to becouse it's static, here and in the respective component
};


}


#endif