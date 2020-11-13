
#ifndef COR_GROUP_CLIENT_HPP
#define COR_GROUP_CLIENT_HPP

#include <hpx/hpx.hpp>

#include "cor/resources/group.hpp"
#include "cor/elements/dynamic_organizer.hpp"


namespace cor {

struct Group_Client: hpx::components::client_base<Group_Client, Group>
{

private:
	static hpx::future<hpx::id_type> create_server(idp_t idp, std::string const& module) {
		return hpx::local_new<Group>(idp, module);
	}
	static hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, std::string const& module) {
		return hpx::new_<Group>(locality, idp, module);
	}

public:
	typedef hpx::components::client_base<Group_Client, Group> base_type;

	friend class hpx::serialization::access;

	typedef DynamicOrganizer organizer;

	/// Default construct an empty client side representation (not
	/// connected to any existing component). Also needed for serialization
	Group_Client()
	{}

	/// Create a client side representation for the existing
	/// Closure_Component instance with the given GID
	Group_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Group_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Group_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	// Construtor para réplicas
	Group_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Group_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Group_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	/// Standard constructor with parameters
	Group_Client(idp_t idp, std::string const& module) :
		base_type(create_server(idp, module)),
		_idp(idp)
	{}

	Group_Client(idp_t idp, hpx::id_type locality, std::string const& module) :
		base_type(create_server_remote(idp, locality, module)),
		_idp(idp)
	{}


	/** Resource interface **/
	// método que retorna o idp global do recurso, que está presente na classe Resource
	idp_t IdpGlobal()
	{
		typedef ResourceNonMigrable::Idp_action_ResourceNonMigrable action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	hpx::id_type GetLocalityGID()
	{
		typedef ResourceNonMigrable::GetLocalityGID_action_ResourceNonMigrable action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}
	
	unsigned int GetLocalityID()
	{
		typedef ResourceNonMigrable::GetLocalityID_action_ResourceNonMigrable action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}
	


	/** Dynamic organizer interface **/
	void Join(idp_t idp, std::string const& name)
	{
	  typedef Group::Join_action_Group action_type;
	  return hpx::async<action_type>(this->get_id(), idp, name).get();
	}

	void Leave(idp_t idp)
	{
	  typedef Group::Leave_action_Group action_type;
	  return hpx::async<action_type>(this->get_id(), idp).get();
	}

	std::string GetModuleName()
	{
		typedef Group::GetModuleName_action_Group action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	std::size_t GetTotalMembers()
	{
		typedef Group::GetTotalMembers_action_Group action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	std::vector<idp_t> GetMemberList()
	{
		typedef Group::GetMemberList_action_Group action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

    idp_t GetIdp(idm_t idm)
	{
		typedef Group::GetIdp1_action_Group action_type;
		return hpx::async<action_type>(this->get_id(), idm).get();
	}

    idp_t GetIdp(std::string const& name)
	{
		typedef Group::GetIdp2_action_Group action_type;
		return hpx::async<action_type>(this->get_id(), name).get();
	}

    idm_t GetIdm(idp_t idp)
	{
		typedef Group::GetIdm1_action_Group action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}

    idm_t GetIdm(std::string const& name)
	{
		typedef Group::GetIdm2_action_Group action_type;
		return hpx::async<action_type>(this->get_id(), name).get();
	}

    idm_t GetDynamicOrganizerIdp()
	{
		typedef Group::GetDynamicOrganizerIdp_action_Group action_type;
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
		return 2;
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