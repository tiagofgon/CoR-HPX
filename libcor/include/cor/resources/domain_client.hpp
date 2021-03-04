
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
		_idp(IdpGlobal()),
		_idp_agent(0)
	{}

	Domain_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal()),
		_idp_agent(0)
	{}

	Domain_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal()),
		_idp_agent(0)
	{}

	// Constructor for replicas
	Domain_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp),
		_idp_agent(0)
	{}

	Domain_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp),
		_idp_agent(0)
	{}

	Domain_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp),
		_idp_agent(0)
	{}

	// Standard constructor with parameters
	Domain_Client(idp_t idp, unsigned int pod_id, std::string const& module) :
		base_type(create_server(idp, pod_id, module)),
		_idp(idp),
		_idp_agent(0)
	{
		global::pods[pod_id]->setDomainIdp(idp); // Update domain idp variable's Pod
	}

	// This can only be done when remote domains will be implemented!
	Domain_Client(idp_t idp, unsigned int pod_id, hpx::id_type locality, std::string const& module) :
		base_type(create_server_remote(idp, pod_id, locality, module)),
		_idp(idp),
		_idp_agent(0)
	{
		global::pods[pod_id]->setDomainIdp(idp); // Update domain idp variable's Pod
	}




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


	/** Dynamic organizer's interface **/
	hpx::future<void> Join(hpx::launch::async_policy, idp_t idp, std::string const& name)
	{
		typedef Domain::Join_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), idp, name);
	}

	void Join(idp_t idp, std::string const& name)
	{
		typedef Domain::Join_action_Domain action_type;
		return action_type()(this->get_id(), idp, name);	
	}

	hpx::future<void> Leave(hpx::launch::async_policy, idp_t idp)
	{
		typedef Domain::Leave_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), idp);
	}

	void Leave(idp_t idp)
	{
		typedef Domain::Leave_action_Domain action_type;
		return action_type()(this->get_id(), idp);
	}

	hpx::future<std::string> GetModuleName(hpx::launch::async_policy)
	{
		typedef Domain::GetModuleName_action_Domain action_type;
		return hpx::async<action_type>(this->get_id());
	}

	std::string GetModuleName()
	{
		typedef Domain::GetModuleName_action_Domain action_type;
		return action_type()(this->get_id());
	}

	hpx::future<std::size_t> GetTotalMembers(hpx::launch::async_policy)
	{
		typedef Domain::GetTotalMembers_action_Domain action_type;
		return hpx::async<action_type>(this->get_id());
	}

	std::size_t GetTotalMembers()
	{
		typedef Domain::GetTotalMembers_action_Domain action_type;
		return action_type()(this->get_id());
	}

	hpx::future<std::vector<idp_t>> GetMemberList(hpx::launch::async_policy)
	{
		typedef Domain::GetMemberList_action_Domain action_type;
		return hpx::async<action_type>(this->get_id());
	}

	std::vector<idp_t> GetMemberList()
	{
		typedef Domain::GetMemberList_action_Domain action_type;
		return action_type()(this->get_id());
	}

    hpx::future<idp_t> GetIdp(hpx::launch::async_policy, idm_t idm)
	{
		typedef Domain::GetIdp1_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), idm);
	}

    idp_t GetIdp(idm_t idm)
	{
		typedef Domain::GetIdp1_action_Domain action_type;
		return action_type()(this->get_id(), idm);
	}

    hpx::future<idp_t> GetIdp(hpx::launch::async_policy, std::string const& name)
	{
		typedef Domain::GetIdp2_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), name);
	}

    idp_t GetIdp(std::string const& name)
	{
		typedef Domain::GetIdp2_action_Domain action_type;
		return action_type()(this->get_id(), name);	
	}

    hpx::future<idm_t> GetIdm(hpx::launch::async_policy, idp_t idp)
	{
	  	typedef Domain::GetIdm1_action_Domain action_type;
	  	return hpx::async<action_type>(this->get_id(), idp);
	}

    idm_t GetIdm(idp_t idp)
	{
	  	typedef Domain::GetIdm1_action_Domain action_type;
	  	return action_type()(this->get_id(), idp);
	}

    hpx::future<idm_t> GetIdm(hpx::launch::async_policy, std::string const& name)
	{
		typedef Domain::GetIdm2_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), name);
	}

    idm_t GetIdm(std::string const& name)
	{
		typedef Domain::GetIdm2_action_Domain action_type;
		return action_type()(this->get_id(), name);
	}

    hpx::future<idm_t> GetDynamicOrganizerIdp(hpx::launch::async_policy)
	{
		typedef Domain::GetDynamicOrganizerIdp_action_Domain action_type;
		return hpx::async<action_type>(this->get_id());
	}

    idm_t GetDynamicOrganizerIdp()
	{
		typedef Domain::GetDynamicOrganizerIdp_action_Domain action_type;
		return action_type()(this->get_id());
	}




	/** Container's interface **/
    hpx::future<std::string> GetGlobalContext(hpx::launch::async_policy)
	{
		typedef Domain::GetGlobalContext_action_Domain action_type;
		return hpx::async<action_type>(this->get_id());
	}

    std::string GetGlobalContext()
	{
		typedef Domain::GetGlobalContext_action_Domain action_type;
		return action_type()(this->get_id());
	}

    hpx::future<std::string> GetLocalContext(hpx::launch::async_policy)
	{
		typedef Domain::GetLocalContext_action_Domain action_type;
		return hpx::async<action_type>(this->get_id());
	}
	
    std::string GetLocalContext()
	{
		typedef Domain::GetLocalContext_action_Domain action_type;
		return action_type()(this->get_id());
	}

	hpx::future<unsigned int> GetNumPods(hpx::launch::async_policy)
	{
		typedef Domain::GetNumPods_action_Domain action_type;
		return hpx::async<action_type>(this->get_id());
	}

	unsigned int GetNumPods()
	{
		typedef Domain::GetNumPods_action_Domain action_type;
		return action_type()(this->get_id());
	}

	hpx::future<unsigned int> GetNumDomains(hpx::launch::async_policy)
	{
		typedef Domain::GetNumDomains_action_Domain action_type;
		return hpx::async<action_type>(this->get_id());
	}

	unsigned int GetNumDomains()
	{
		typedef Domain::GetNumDomains_action_Domain action_type;
		return action_type()(this->get_id());
	}

	hpx::future<std::vector<idp_t>> GetPods(hpx::launch::async_policy)
	{
		return hpx::async([&](){
			typedef Domain::GetPods_action_Domain action_type;
			auto vec =  hpx::async<action_type>(this->get_id()).get();

			for(int i=0; i<vec.size(); i++) {
				if(vec[i]==0) {
					vec.erase(vec.begin() + i);
					return vec;
				}
			}
			return vec;
		});
	}

	std::vector<idp_t> GetPods()
	{
		typedef Domain::GetPods_action_Domain action_type;
		auto vec = action_type()(this->get_id());

		for(int i=0; i<vec.size(); i++) {
			if(vec[i]==0) {
				vec.erase(vec.begin() + i);
				return vec;
			}
		}
		return vec;
	}

	hpx::future<std::vector<idp_t>> GetDomains(hpx::launch::async_policy)
	{
		return hpx::async([&](){
			typedef Domain::GetDomains_action_Domain action_type;
			auto vec =  hpx::async<action_type>(this->get_id()).get();

			for(int i=0; i<vec.size(); i++) {
				if(vec[i]==0) {
					vec.erase(vec.begin() + i);
					return vec;
				}
			}
			return vec;
		});
	}

	std::vector<idp_t> GetDomains()
	{
		typedef Domain::GetDomains_action_Domain action_type;
		auto vec = action_type()(this->get_id());

		for(int i=0; i<vec.size(); i++) {
			if(vec[i]==0) {
				vec.erase(vec.begin() + i);
				return vec;
			}
		}
		return vec;
	}

	hpx::future<std::vector<idp_t>> GetRemotePods(hpx::launch::async_policy)
	{
		return hpx::async([&](){
			auto vec = GetPods();
			for(int i=0; i<vec.size(); i++) {
				if(vec[i]==_idp) {
					vec.erase(vec.begin() + i);
					return vec;
				}
			}
			return vec;
		});
	}

	std::vector<idp_t> GetRemotePods()
	{
		auto vec = GetPods();
		for(int i=0; i<vec.size(); i++) {
			if(vec[i]==_idp) {
				vec.erase(vec.begin() + i);
				return vec;
			}
		}
		return vec;
	}

	hpx::future<std::vector<idp_t>> GetRemoteDomains(hpx::launch::async_policy)
	{
		return hpx::async([&](){
			auto vec = GetDomains();
			for(int i=0; i<vec.size(); i++) {
				if(vec[i]==_idp) {
					vec.erase(vec.begin() + i);
					return vec;
				}
			}
			return vec;
		});
	}

	std::vector<idp_t> GetRemoteDomains()
	{
		auto vec = GetDomains();
		for(int i=0; i<vec.size(); i++) {
			if(vec[i]==_idp) {
				vec.erase(vec.begin() + i);
				return vec;
			}
		}
		return vec;
	}

	hpx::future<idp_t> GetActiveResourceIdp(hpx::launch::async_policy)
	{
		// primeira chamada desta funcao, dentro do cor GetDomain()
		if(_idp_agent == 0) {
			auto id = hpx::threads::get_self_id();
			uint64_t* ptr=(uint64_t*) &id;
			auto ptr2 = (*ptr);
			// std::cout << "Thread HPX - GetActiveResourceIdp: " << ptr2 << std::endl;
			return hpx::async([&](){
				typedef cor::Domain::GetActiveResourceIdp_action_Domain action_type;
				auto res = hpx::async<action_type>(this->get_id(), ptr2).get(); 
				_idp_agent = res;
				return _idp_agent;
			});

		}
		return hpx::make_ready_future(_idp_agent);
	}

	idp_t GetActiveResourceIdp()
	{
		// primeira chamada desta funcao, dentro do cor GetDomain()
		if(_idp_agent == 0) {
			auto id = hpx::threads::get_self_id();
			uint64_t* ptr=(uint64_t*) &id;
			auto ptr2 = (*ptr);
			// std::cout << "Thread HPX - GetActiveResourceIdp: " << ptr2 << std::endl;

			typedef cor::Domain::GetActiveResourceIdp_action_Domain action_type;
			auto res = hpx::async<action_type>(this->get_id(), ptr2).get(); 
			_idp_agent = res;
			return _idp_agent;

		}
		return _idp_agent;
	}

	hpx::future<idp_t> GetPredecessorIdp(hpx::launch::async_policy, idp_t idp)
	{
		typedef cor::Domain::GetPredecessorIdp_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), idp); 
	}

	idp_t GetPredecessorIdp(idp_t idp)
	{
		typedef cor::Domain::GetPredecessorIdp_action_Domain action_type;
		return action_type()(this->get_id(), idp); 	
	}

	template <typename T>
	hpx::future<std::unique_ptr<T>> GetLocalResource(hpx::launch::async_policy, idp_t idp)
	{
		typedef cor::Domain::GetLocalResource_action_Domain<T> action_type;
		return hpx::async<action_type>(this->get_id(), idp); 
	}

	template <typename T>
	std::unique_ptr<T> GetLocalResource(idp_t idp)
	{
		typedef cor::Domain::GetLocalResource_action_Domain<T> action_type;
		return action_type()(this->get_id(), idp); 
	}

	// Funcao para apagar
	template <typename T, typename ... Args>
	hpx::future<std::unique_ptr<T>> CreateLocal_agent(hpx::launch::async_policy, idp_t ctx, std::string const& name, Args ... args)
	{
		typedef cor::Domain::CreateLocal_agent_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, args...);
	}

	template <typename T, typename ... Args>
	hpx::future<std::unique_ptr<T>> CreateLocal(hpx::launch::async_policy, idp_t ctx, std::string const& name, Args ... args)
	{
		typedef cor::Domain::CreateLocal_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, args...);
	}

	template <typename T, typename ... Args>
	std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, Args ... args)
	{
		std::shared_ptr<Domain> ptr = hpx::get_ptr<Domain>(hpx::launch::sync, this->get_id());
		return ptr->CreateLocal<T, Args...>(ctx, name, args...);
	}

	template <typename T, typename ... Args>
	hpx::future<idp_t> Create(hpx::launch::async_policy, idp_t ctx, std::string const& name, Args ... args)
	{
		typedef cor::Domain::Create_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, args...);
	}

	template <typename T, typename ... Args>
	idp_t Create(idp_t ctx, std::string const& name, Args ... args)
	{
		std::shared_ptr<Domain> ptr = hpx::get_ptr<Domain>(hpx::launch::sync, this->get_id());
		idp_t res = ptr->Create<T, Args...>(ctx, name, args...);
		return res;
	}

    template <typename T, typename ... Args>
    hpx::future<idp_t> CreateRemote(hpx::launch::async_policy, idp_t ctx, std::string const& name, Args ... args)
	{
		typedef cor::Domain::CreateRemote_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, args...);
	}

    template <typename T, typename ... Args>
    idp_t CreateRemote(idp_t ctx, std::string const& name, Args ... args)
	{
		std::shared_ptr<Domain> ptr = hpx::get_ptr<Domain>(hpx::launch::sync, this->get_id());
		return ptr->CreateRemote<T, Args...>(ctx, name, args...);
	}

    template <typename T>
    hpx::future<std::unique_ptr<T>> CreateReference(hpx::launch::async_policy, idp_t idp, idp_t ctx, std::string const& name)
	{
		typedef cor::Domain::CreateReference_action_Domain<T> action_type;
		return hpx::async<action_type>(this->get_id(), idp, ctx, name);
	}

    template <typename T>
    std::unique_ptr<T> CreateReference(idp_t idp, idp_t ctx, std::string const& name)
	{
		typedef cor::Domain::CreateReference_action_Domain<T> action_type;
		return action_type()(this->get_id(), idp, ctx, name);
	}

	template <typename T, typename ... Args>
	hpx::future<std::unique_ptr<T>> CreateCollective(hpx::launch::async_policy, idp_t ctx, std::string const& name, unsigned int total_members, Args ... args)
	{
		typedef cor::Domain::CreateCollective1_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, total_members, args...);
	}

	template <typename T, typename ... Args>
	std::unique_ptr<T> CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args ... args)
	{
		//return CreateCollective_action<T, Args...>(ctx, name, total_members, std::forward<Args>(args)...);
		std::shared_ptr<Domain> ptr = hpx::get_ptr<Domain>(hpx::launch::sync, this->get_id());
		return ptr->CreateCollective1<T, Args...>(ctx, name, total_members, args...);
	}

    template <typename T, typename ... Args>
    hpx::future<std::unique_ptr<T>> CreateCollective(hpx::launch::async_policy, idp_t clos, idp_t ctx, std::string const& name, Args ... args)
	{
		auto active_rsc_idp = GetActiveResourceIdp(hpx::launch::async).get(); // I'll get the idp of the current agent that will be needed in the Pod
		typedef cor::Domain::CreateCollective2_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), active_rsc_idp, clos, ctx, name, args...);
	}

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective(idp_t clos, idp_t ctx, std::string const& name, Args ... args)
	{
		//return CreateCollective_action<T, Args...>(clos, ctx, name, std::forward<Args>(args)...);
		auto active_rsc_idp = GetActiveResourceIdp(); // I'll get the idp of the current agent that will be needed in the Pod
		std::shared_ptr<Domain> ptr = hpx::get_ptr<Domain>(hpx::launch::sync, this->get_id());
		return ptr->CreateCollective2<T, Args...>(active_rsc_idp, clos, ctx, name, args...);
	}

	// return a future, the funcion Run from executor return allways a future. 
	// in this case, we do .get(), but this get stands for async's future, not Run'future
    template <typename T, typename ... Args>
    auto Run(hpx::launch::async_policy, idp_t idp, Args... args)
	{
		typedef cor::Domain::Run_action_Domain<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), idp, args...).get();
	}

	// return a future
    template <typename T, typename ... Args>
    auto Run(idp_t idp, Args... args)
	{
		typedef cor::Domain::Run_action_Domain<T, Args...> action_type;
		return action_type()(this->get_id(), idp, args...);
	}

	hpx::future<idp_t> Spawn(hpx::launch::async_policy, std::string const& context, unsigned int npods, unsigned int total_pods, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
	{
		auto parent = GetActiveResourceIdp(hpx::launch::async).get();
		typedef cor::Domain::Spawn_action_Domain action_type;
		return hpx::async<action_type>(this->get_id(), context, npods, total_pods, parent, module, args, hosts); 
	}

	idp_t Spawn(std::string const& context, unsigned int npods, unsigned int total_pods, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
	{
		auto parent = GetActiveResourceIdp();
		typedef cor::Domain::Spawn_action_Domain action_type;
		return action_type()(this->get_id(), context, npods, total_pods, parent, module, args, hosts); 
	}




	/** Domain's interface **/
	unsigned int GetPodId()
	{
		typedef Domain::GetPodId_action_Domain action_type;
		return action_type()(this->get_id());
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
		6 - Data
		7 - Barrier
		8 - Mutex
		9 - RWMutex
		*/
		return hpx::make_ready_future(1);
	}

	int GetComponentType()
	{
		return 1;
	}

	// For compilation purposes only, it is never used here!
	hpx::future<hpx::id_type> GetMailboxGid(hpx::launch::async_policy) {
		return hpx::make_ready_future(hpx::find_here());
	}

	hpx::id_type GetMailboxGid() {
		return hpx::find_here();
	}
	

private:
	hpx::future<hpx::id_type> create_server(idp_t idp, unsigned int pod_id, std::string const& module) {
		return hpx::local_new<Domain>(idp, pod_id, module);
	}
	hpx::future<hpx::id_type> create_server_remote(idp_t idp, unsigned int pod_id, hpx::id_type locality, std::string const& module) {
		return hpx::new_<Domain>(locality, idp, pod_id, module);
	}

	template <typename Archive>
	void serialize(Archive& ar, unsigned) {
		ar & _idp;
	}

	idp_t _idp; // local idp
	idp_t _idp_agent; // idp do agent que arranca o código do dominio

};


}


#endif