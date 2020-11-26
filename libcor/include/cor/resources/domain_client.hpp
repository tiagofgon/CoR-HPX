
#ifndef COR_DOMAIN_CLIENT_HPP
#define COR_DOMAIN_CLIENT_HPP

#include "cor/resources/domain.hpp"
#include "cor/system/system.hpp"

#include <hpx/hpx.hpp>


namespace cor {

class Domain_Client : hpx::components::client_base<Domain_Client, Domain>
{

public:
	typedef hpx::components::client_base<Domain_Client, Domain> base_type;

	friend class hpx::serialization::access;

	typedef DynamicOrganizer organizer;

	// Default construct an empty client side representation (not
	// connected to any existing component). Also needed for serialization
	Domain_Client()
	{}

	// Create a client side representation for the existing
	// Closure instance with the given GID
	Domain_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Domain_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Domain_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	// Constructor for replicas
	Domain_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Domain_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Domain_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	// Standard constructor with parameters
	Domain_Client(idp_t idp, std::string const& module) :
		base_type(create_server(idp, module)),
		_idp(idp)
	{
		global::pod->setDomainIdp(idp); // Update domain idp variable's Pod
	}

	// This can only be done when remote domains will be implemented!
	Domain_Client(idp_t idp, hpx::id_type locality, std::string const& module) :
		base_type(create_server_remote(idp, locality, module)),
		_idp(idp)
	{
		global::pod->setDomainIdp(idp); // Update domain idp variable's Pod
	}


	/** Resource's interface **/
	// method that returns the global idp of the resource, which is present in the class Resource
	idp_t IdpGlobal()
	{
		typedef Resource::Idp_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	// method that returns the GID(hpx::id_type) of this resource locality
	hpx::id_type GetLocalityGID()
	{
		typedef Resource::GetLocalityGID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}

	// method that returns the number of this resource locality
	unsigned int GetLocalityID()
	{
		typedef Resource::GetLocalityID_action_Resource action_type;
		return hpx::async<action_type>(base_type::get_id()).get();
	}


	/** Dynamic organizer's interface **/
	void Join(idp_t idp, std::string const& name)
	{
		typedef Domain::Join_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), idp, name).get();
	}

	void Leave(idp_t idp)
	{
		typedef Domain::Leave_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}

	std::string GetModuleName()
	{
		typedef Domain::GetModuleName_action_Domain action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	std::size_t GetTotalMembers()
	{
		typedef Domain::GetTotalMembers_action_Domain action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	std::vector<idp_t> GetMemberList()
	{
		typedef Domain::GetMemberList_action_Domain action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

    idp_t GetIdp(idm_t idm)
	{
		typedef Domain::GetIdp1_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), idm).get();
	}

    idp_t GetIdp(std::string const& name)
	{
		typedef Domain::GetIdp2_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), name).get();
	}

    idm_t GetIdm(idp_t idp)
	{
	  	typedef Domain::GetIdm1_action_Domain action_type;
	  	return hpx::async<action_type>(this->get_id(), idp).get();
	}

    idm_t GetIdm(std::string const& name)
	{
		typedef Domain::GetIdm2_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), name).get();
	}

    idm_t GetDynamicOrganizerIdp()
	{
		typedef Domain::GetDynamicOrganizerIdp_action_Domain action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}


	/** Container's interface **/
    std::string GetGlobalContext()
	{
		typedef Domain::GetGlobalContext_action_Domain action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

    std::string GetLocalContext()
	{
		typedef Domain::GetLocalContext_action_Domain action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

    unsigned int GetTotalPods()
	{
		typedef Domain::GetTotalPods_action_Domain action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

    unsigned int GetTotalDomains()
	{
		typedef Domain::GetTotalDomains_action_Domain action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	idp_t GetActiveResourceIdp()
	{
		auto id = hpx::threads::get_self_id();
		uint64_t* ptr=(uint64_t*) &id;
		auto ptr2 = (*ptr);
		// std::cout << "Thread HPX - GetActiveResourceIdp: " << ptr2 << std::endl;

		typedef cor::Domain::GetActiveResourceIdp_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), ptr2).get(); 
	}

	idp_t GetPredecessorIdp(idp_t idp)
	{
		typedef cor::Domain::GetPredecessorIdp_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), idp).get(); 
	}

	template <typename T>
	std::unique_ptr<T> GetLocalResource(idp_t idp)
	{
		typedef cor::Domain::GetLocalResource_action_Domain<T> action_type;
		return hpx::async<action_type>(this->get_id(), idp).get(); 
	}

	template <typename T, typename ... Args>
	std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, Args&& ... args)
	{
		typedef cor::Domain::CreateLocal_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
	}

	template <typename T, typename ... Args>
	idp_t Create(idp_t ctx, std::string const& name, Args&& ... args)
	{
		typedef cor::Domain::Create_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
	}

    template <typename T, typename ... Args>
    idp_t CreateRemote(idp_t ctx, std::string const& name, Args&& ... args)
	{
		typedef cor::Domain::CreateRemote_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
	}

    template <typename T>
    std::unique_ptr<T> CreateReference(idp_t idp, idp_t ctx, std::string const& name)
	{
		typedef cor::Domain::CreateReference_action_Domain<T> action_type;
		return hpx::async<action_type>(this->get_id(), idp, ctx, name).get();
	}

	template <typename T, typename ... Args>
	std::unique_ptr<T> CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args)
	{
		typedef cor::Domain::CreateCollective1_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, total_members, std::forward<Args>(args)...).get();
	}

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective(idp_t clos, idp_t ctx, std::string const& name, Args&& ... args)
	{
		auto active_rsc_idp = GetActiveResourceIdp(); // I'll get the idp of the current agent that will be needed in the Pod
		typedef cor::Domain::CreateCollective2_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), active_rsc_idp, clos, ctx, name, std::forward<Args>(args)...).get();
	}

    template <typename T, typename ... Args>
    void Run(idp_t idp, Args&&... args)
	{
		typedef cor::Domain::Run_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), idp, std::forward<Args>(args)...).get();
	}

    template <typename T>
    void Wait(idp_t idp)
	{
		typedef cor::Domain::Wait_action_Domain<T> action_type;
		return hpx::async<action_type>(this->get_id(), idp).get(); 
	}

    template <typename T>
    auto Get(idp_t idp)
	{
		typedef cor::Domain::Get_action_Domain<T> action_type;
		return hpx::async<action_type>(this->get_id(), idp).get(); 
	}

	idp_t Spawn(std::string const& context, unsigned int npods, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
	{
		auto parent = GetActiveResourceIdp();
		typedef cor::Domain::Spawn_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), context, npods, parent, module, args, hosts).get(); 
	}


	/** Local Client's interface **/
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
		9 - RWMutex
		*/
		return 1;
	}

	// For compilation purposes only, it is never used here!
	hpx::id_type GetMailboxGid() {
		return hpx::find_here();
	}
	

private:
	hpx::future<hpx::id_type> create_server(idp_t idp, std::string const& module) {
		return hpx::local_new<Domain>(idp, module);
	}

	hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, std::string const& module) {
		return hpx::new_<Domain>(locality, idp, module);
	}

	template <typename Archive>
	void serialize(Archive& ar, unsigned) {
		ar & _idp;
	}

	idp_t _idp; // local idp

};


}


#endif