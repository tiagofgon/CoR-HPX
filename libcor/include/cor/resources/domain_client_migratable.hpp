
// #ifndef COR_DOMAIN_CLIENT_HPP
// #define COR_DOMAIN_CLIENT_HPP

// #include <hpx/hpx.hpp>

// #include "cor/resources/domain.hpp"
// #include "cor/resources/teste.hpp"
// #include "cor/system/system.hpp"

// namespace cor {

// class Domain_Client : hpx::components::client_base<Domain_Client, Domain>
// {

// private:
// 	static hpx::future<hpx::id_type> create_server(idp_t idp, std::string const& module) {
// 		return hpx::local_new<Domain>(idp, module);
// 	}
// 	static hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, std::string const& module) {
// 		return hpx::new_<Domain>(locality, idp, module);
// 	}

// 	// typedef typename Domain::data_type data_type;
	

// public:
// 	typedef hpx::components::client_base<Domain_Client, Domain> base_type;

// 	friend class hpx::serialization::access;
// 	friend class Domain;

// 	typedef DynamicOrganizer organizer;

// 	friend class ResourceManager;
// 	/// Default construct an empty client side representation (not
// 	/// connected to any existing component). Also needed for serialization
// 	Domain_Client()
// 	{}

// 	/// Create a client side representation for the existing
// 	/// Closure instance with the given GID
// 	Domain_Client(hpx::future<hpx::id_type> && id) :
// 		base_type(std::move(id)),
// 		_idp(IdpGlobal())
// 	{}

// 	Domain_Client(hpx::shared_future<hpx::id_type> && id) :
// 		base_type(std::move(id)),
// 		_idp(IdpGlobal())
// 	{}

// 	Domain_Client(hpx::id_type && id) :
// 		base_type(std::move(id)),
// 		_idp(IdpGlobal())
// 	{}

// 	// Construtor para réplicas
// 	Domain_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
// 		base_type(std::move(id)),
// 		_idp(idp)
// 	{}

// 	Domain_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
// 		base_type(std::move(id)),
// 		_idp(idp)
// 	{}

// 	Domain_Client(idp_t idp, hpx::id_type && id) :
// 		base_type(std::move(id)),
// 		_idp(idp)
// 	{}

// 	/// Standard constructor with parameters
// 	Domain_Client(idp_t idp, std::string const& module) :
// 		base_type(create_server(idp, module)),
// 		_idp(idp)
// 	{
// 		global::pod->setDomainIdp(idp); // Update domain idp variable's Pod
// 	}

// 	// Isto só é possivel fazer quando se implementar dominios remotos!
// 	Domain_Client(idp_t idp, hpx::id_type locality, std::string const& module) :
// 		base_type(create_server_remote(idp, locality, module)),
// 		_idp(idp)
// 	{
// 		global::pod->setDomainIdp(idp); // Update domain idp variable's Pod
// 	}


// 	/** Resource interface **/
// 	// método que retorna o idp global do recurso, que está presente na classe Resource
// 	idp_t IdpGlobal()
// 	{
// 		typedef Resource::Idp_action_Resource action_type;
// 		return hpx::async<action_type>(base_type::get_id()).get();
// 	}

// 	idp_t IdpGlobal_here()
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Resource::Idp_action_Resource action_type;
// 		return hpx::async<action_type>(base_type::get_id()).get();
// 	}

// 	hpx::id_type GetLocalityGID()
// 	{
// 		typedef Resource::GetLocalityGID_action_Resource action_type;
// 		return hpx::async<action_type>(base_type::get_id()).get();
// 	}

// 	hpx::id_type GetLocalityGID_here()
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Resource::GetLocalityGID_action_Resource action_type;
// 		return hpx::async<action_type>(base_type::get_id()).get();
// 	}

// 	unsigned int GetLocalityID()
// 	{
// 		typedef Resource::GetLocalityID_action_Resource action_type;
// 		return hpx::async<action_type>(base_type::get_id()).get();
// 	}

// 	unsigned int GetLocalityID_here()
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Resource::GetLocalityID_action_Resource action_type;
// 		return hpx::async<action_type>(base_type::get_id()).get();
// 	}


// 	/** Dynamic organizer interface **/
// 	void Join(idp_t idp, std::string const& name)
// 	{
// 		typedef Domain::Join_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), idp, name).get();
// 	}

// 	void Join_here(idp_t idp, std::string const& name)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::Join_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), idp, name).get();
// 	}

// 	void Leave(idp_t idp)
// 	{
// 		typedef Domain::Leave_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), idp).get();
// 	}

// 	void Leave_here(idp_t idp)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::Leave_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), idp).get();
// 	}

// 	std::string GetModuleName()
// 	{
// 		typedef Domain::GetModuleName_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

// 	std::string GetModuleName_here()
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::GetModuleName_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

// 	std::size_t GetTotalMembers()
// 	{
// 		typedef Domain::GetTotalMembers_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

// 	std::size_t GetTotalMembers_here()
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::GetTotalMembers_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

// 	std::vector<idp_t> GetMemberList()
// 	{
// 		typedef Domain::GetMemberList_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

// 	std::vector<idp_t> GetMemberList_here()
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::GetMemberList_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

//     idp_t GetIdp(idm_t idm)
// 	{
// 		typedef Domain::GetIdp1_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), idm).get();
// 	}

//     idp_t GetIdp_here(idm_t idm)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::GetIdp1_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), idm).get();
// 	}

//     idp_t GetIdp(std::string const& name)
// 	{
// 	  typedef Domain::GetIdp2_action_Domain action_type;
// 	  return hpx::async<action_type>(this->get_id(), name).get();
// 	}

//     idp_t GetIdp_here(std::string const& name)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::GetIdp2_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), name).get();
// 	}

//     idm_t GetIdm(idp_t idp)
// 	{
// 	  typedef Domain::GetIdm1_action_Domain action_type;
// 	  return hpx::async<action_type>(this->get_id(), idp).get();
// 	}

//     idm_t GetIdm_here(idp_t idp)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::GetIdm1_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), idp).get();
// 	}

//     idm_t GetIdm(std::string const& name)
// 	{
// 	  typedef Domain::GetIdm2_action_Domain action_type;
// 	  return hpx::async<action_type>(this->get_id(), name).get();
// 	}

//     idm_t GetIdm_here(std::string const& name)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::GetIdm2_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), name).get();
// 	}

//     idm_t GetDynamicIdp()
// 	{
// 	  typedef Domain::GetDynamicIdp_action_Domain action_type;
// 	  return hpx::async<action_type>(this->get_id()).get();
// 	}
	
//     idm_t GetDynamicIdp_here()
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::GetDynamicIdp_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}




// 	/** Container interface **/
//     std::string GetGlobalContext()
// 	{
// 		typedef Domain::GetGlobalContext_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

//     std::string GetGlobalContext_here()
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::GetGlobalContext_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

//     std::string GetLocalContext()
// 	{
// 		typedef Domain::GetLocalContext_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

//     std::string GetLocalContext_here()
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::GetLocalContext_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

//     unsigned int GetTotalPods()
// 	{
// 		typedef Domain::GetTotalPods_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

//     unsigned int GetTotalPods_here()
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::GetTotalPods_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

//     unsigned int GetTotalDomains()
// 	{
// 		typedef Domain::GetTotalDomains_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

//     unsigned int GetTotalDomains_here()
// 	{
// 		Migrate(hpx::find_here());
// 		typedef Domain::GetTotalDomains_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id()).get();
// 	}

// 	idp_t GetActiveResourceIdp()
// 	{
// 		auto id = hpx::threads::get_self_id();
// 		uint64_t* ptr=(uint64_t*) &id;
// 		auto ptr2 = (*ptr);
// 		// std::cout << "Thread HPX - GetActiveResourceIdp: " << ptr2 << std::endl;

// 		typedef cor::Domain::GetActiveResourceIdp_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), ptr2).get(); 
// 	}

// 	idp_t GetActiveResourceIdp_here()
// 	{
// 		Migrate(hpx::find_here());
// 		auto id = hpx::threads::get_self_id();
// 		uint64_t* ptr=(uint64_t*) &id;
// 		auto ptr2 = (*ptr);
// 		// std::cout << "Thread HPX - GetActiveResourceIdp: " << ptr2 << std::endl;

// 		typedef cor::Domain::GetActiveResourceIdp_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), ptr2).get(); 
// 	}

// 	idp_t GetPredecessorIdp(idp_t idp)
// 	{
// 		typedef cor::Domain::GetPredecessorIdp_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), idp).get(); 
// 	}

// 	idp_t GetPredecessorIdp_here(idp_t idp)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef cor::Domain::GetPredecessorIdp_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), idp).get(); 
// 	}

// 	template <typename T>
// 	std::unique_ptr<T> GetLocalResource(idp_t idp)
// 	{
// 		typedef cor::Domain::GetLocalResource_action_Domain<T> action_type;
// 		return hpx::async<action_type>(this->get_id(), idp).get(); 
// 	}

// 	template <typename T>
// 	std::unique_ptr<T> GetLocalResource_here(idp_t idp)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef cor::Domain::GetLocalResource_action_Domain<T> action_type;
// 		return hpx::async<action_type>(this->get_id(), idp).get(); 
// 	}

// 	template <typename T, typename ... Args>
// 	std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, Args&& ... args)
// 	{
// 		typedef cor::Domain::CreateLocal_action_Domain<T, Args...> action_type;
// 		return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
// 	}

// 	template <typename T, typename ... Args>
// 	std::unique_ptr<T> CreateLocal_here(idp_t ctx, std::string const& name, Args&& ... args)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef cor::Domain::CreateLocal_action_Domain<T, Args...> action_type;
// 		return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
// 	}

//     template <typename T, typename ... Args>
//     idp_t CreateRemote(idp_t ctx, std::string const& name, Args&& ... args)
// 	{
// 		typedef cor::Domain::CreateRemote_action_Domain<T, Args...> action_type;
// 		return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
// 	}

//     template <typename T, typename ... Args>
//     idp_t CreateRemote_here(idp_t ctx, std::string const& name, Args&& ... args)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef cor::Domain::CreateRemote_action_Domain<T, Args...> action_type;
// 		return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
// 	}

//     template <typename T, typename ... Args>
//     idp_t Create(idp_t ctx, std::string const& name, Args&& ... args)
// 	{
// 		std::cout << "Domain_Client::Create" << std::endl;
// 		typedef cor::Domain::Create_action_Domain<T, Args...> action_type;
// 		return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
// 	}

//     template <typename T, typename ... Args>
//     idp_t Create_here(idp_t ctx, std::string const& name, Args&& ... args)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef cor::Domain::Create_action_Domain<T, Args...> action_type;
// 		return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
// 	}

//     template <typename T>
//     std::unique_ptr<T> CreateReference(idp_t idp, idp_t ctx, std::string const& name)
// 	{
// 		typedef cor::Domain::CreateReference_action_Domain<T> action_type;
// 		return hpx::async<action_type>(this->get_id(), idp, ctx, name).get();
// 	}

//     template <typename T>
//     std::unique_ptr<T> CreateReference_here(idp_t idp, idp_t ctx, std::string const& name)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef cor::Domain::CreateReference_action_Domain<T> action_type;
// 		return hpx::async<action_type>(this->get_id(), idp, ctx, name).get();
// 	}

// 	template <typename T, typename ... Args>
// 	std::unique_ptr<T> CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args)
// 	{
// 		typedef cor::Domain::CreateCollective1_action_Domain<T, Args...> action_type;
// 		return hpx::async<action_type>(this->get_id(), ctx, name, total_members, std::forward<Args>(args)...).get();
// 	}

// 	template <typename T, typename ... Args>
// 	std::unique_ptr<T> CreateCollective_here(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef cor::Domain::CreateCollective1_action_Domain<T, Args...> action_type;
// 		return hpx::async<action_type>(this->get_id(), ctx, name, total_members, std::forward<Args>(args)...).get();
// 	}

//     template <typename T, typename ... Args>
//     std::unique_ptr<T> CreateCollective(idp_t clos, idp_t ctx, std::string const& name, Args&& ... args)
// 	{
// 		auto active_rsc_idp = GetActiveResourceIdp(); // vou buscar o idp do agente atual que vai ser necessario no Pod
// 		typedef cor::Domain::CreateCollective2_action_Domain<T, Args...> action_type;
// 		return hpx::async<action_type>(this->get_id(), active_rsc_idp, clos, ctx, name, std::forward<Args>(args)...).get();
// 	}

//     template <typename T, typename ... Args>
//     std::unique_ptr<T> CreateCollective_here(idp_t clos, idp_t ctx, std::string const& name, Args&& ... args)
// 	{
// 		Migrate(hpx::find_here());
// 		auto active_rsc_idp = GetActiveResourceIdp(); // vou buscar o idp do agente atual que vai ser necessario no Pod
// 		typedef cor::Domain::CreateCollective2_action_Domain<T, Args...> action_type;
// 		return hpx::async<action_type>(this->get_id(), active_rsc_idp, clos, ctx, name, std::forward<Args>(args)...).get();
// 	}

//     template <typename T, typename ... Args>
//     void Run(idp_t idp, Args&&... args)
// 	{
// 		typedef cor::Domain::Run_action_Domain<T, Args...> action_type;
// 		return hpx::async<action_type>(this->get_id(), idp, std::forward<Args>(args)...).get();
// 	}

//     template <typename T, typename ... Args>
//     void Run_here(idp_t idp, Args&&... args)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef cor::Domain::Run_action_Domain<T, Args...> action_type;
// 		return hpx::async<action_type>(this->get_id(), idp, std::forward<Args>(args)...).get();
// 	}

//     template <typename T>
//     void Wait(idp_t idp)
// 	{
// 		typedef cor::Domain::Wait_action_Domain<T> action_type;
// 		return hpx::async<action_type>(this->get_id(), idp).get(); 
// 	}

//     template <typename T>
//     void Wait_here(idp_t idp)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef cor::Domain::Wait_action_Domain<T> action_type;
// 		return hpx::async<action_type>(this->get_id(), idp).get(); 
// 	}

//     template <typename T>
//     auto Get(idp_t idp)
// 	{
// 		typedef cor::Domain::Get_action_Domain<T> action_type;
// 		return hpx::async<action_type>(this->get_id(), idp).get(); 
// 	}

//     template <typename T>
//     auto Get_here(idp_t idp)
// 	{
// 		Migrate(hpx::find_here());
// 		typedef cor::Domain::Get_action_Domain<T> action_type;
// 		return hpx::async<action_type>(this->get_id(), idp).get(); 
// 	}

// 	idp_t Spawn(std::string const& context, unsigned int npods, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
// 	{
// 		auto parent = GetActiveResourceIdp();
// 		typedef cor::Domain::Spawn_action_Domain action_type;
// 		return hpx::async<action_type>(this->get_id(), context, npods, parent, module, args, hosts).get(); 
// 	}



// 	/** Local interface **/
// 	// local idp of this resource
// 	idp_t Idp() {
// 		return _idp;
// 	}

// 	// Returns component's GID
// 	hpx::id_type GetGid() {
// 		return this->get_id();
// 	}

// 	int GetComponentType()
// 	{
// 		/* Resource identification
// 		1 - Domain
// 		2 - Group
// 		3 - Clousure
// 		4 - ProtoAgent
// 		5 - Agent
// 		6 - Data
// 		7 - Barrier
// 		8 - Mutex
// 		*/
// 		return 1;
// 	}

// 	void Migrate(hpx::id_type dest)
// 	{
// 		hpx::components::migrate<Domain>(this->get_id(), dest).get();
// 	}


// 	// Só para fins de compilação, não é usado aqui nunca!
// 	hpx::id_type GetMailboxGid() {
// 		return hpx::find_here();
// 	}
	
// private:
// 	template <typename Archive>
// 	void serialize(Archive& ar, unsigned) {
// 		ar & _idp;
// 		// std::cout << "serialized\n";
// 	}

// 	idp_t _idp; // local idp

// };


// }

// #endif