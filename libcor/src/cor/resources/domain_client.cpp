
// #include "cor/resources/domain_client.hpp"


// namespace cor {

// Domain_Client::Domain_Client(hpx::future<hpx::id_type> && id) :
// 	base_type(std::move(id)),
// 	_idp(IdpGlobal())
// {}

// Domain_Client::Domain_Client(hpx::shared_future<hpx::id_type> && id) :
// 	base_type(std::move(id)),
// 	_idp(IdpGlobal())
// {}

// Domain_Client::Domain_Client(hpx::id_type && id) :
// 	base_type(std::move(id)),
// 	_idp(IdpGlobal())
// {}

// /// Construtor para réplicas
// Domain_Client::Domain_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
// 	base_type(std::move(id)),
// 	_idp(idp)
// {}

// Domain_Client::Domain_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
// 	base_type(std::move(id)),
// 	_idp(idp)
// {}

// Domain_Client::Domain_Client(idp_t idp, hpx::id_type && id) :
// 	base_type(std::move(id)),
// 	_idp(idp)
// {}

// /// Standard constructor with parameters
// Domain_Client::Domain_Client(idp_t idp, std::string const& module) :
// 	base_type(create_server(idp, module)),
// 	_idp(idp)
// {
// 	global::pod->setDomainIdp(idp); // Update domain idp variable's Pod
// }

// // Isto só é possivel fazer quando se implementar dominios remotos!
// Domain_Client::Domain_Client(idp_t idp, hpx::id_type locality, std::string const& module) :
// 	base_type(create_server_remote(idp, locality, module)),
// 	_idp(idp)
// {
// 	global::pod->setDomainIdp(idp); // Update domain idp variable's Pod
// }




// /** Resource interface **/
// // método que retorna o idp global do recurso, que está presente na classe Resource
// idp_t Domain_Client::IdpGlobal()
// {
// 	typedef ResourceNonMigrable::Idp_action_ResourceNonMigrable action_type;
// 	return hpx::async<action_type>(base_type::get_id()).get();
// }

// hpx::id_type Domain_Client::GetLocalityGID()
// {
// 	typedef ResourceNonMigrable::GetLocalityGID_action_ResourceNonMigrable action_type;
// 	return hpx::async<action_type>(base_type::get_id()).get();
// }

// unsigned int Domain_Client::GetLocalityID()
// {
// 	typedef ResourceNonMigrable::GetLocalityID_action_ResourceNonMigrable action_type;
// 	return hpx::async<action_type>(base_type::get_id()).get();
// }




// /** Dynamic organizer interface **/
// void Domain_Client::Join(idp_t idp, std::string const& name)
// {
// 	typedef Domain::Join_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id(), idp, name).get();
// }

// void Domain_Client::Leave(idp_t idp)
// {
// 	typedef Domain::Leave_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id(), idp).get();
// }

// std::string Domain_Client::GetModuleName()
// {
// 	typedef Domain::GetModuleName_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id()).get();
// }

// std::size_t Domain_Client::GetTotalMembers()
// {
// 	typedef Domain::GetTotalMembers_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id()).get();
// }

// std::vector<idp_t> Domain_Client::GetMemberList()
// {
// 	typedef Domain::GetMemberList_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id()).get();
// }

// idp_t Domain_Client::GetIdp(idm_t idm)
// {
// 	typedef Domain::GetIdp1_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id(), idm).get();
// }

// idp_t Domain_Client::GetIdp(std::string const& name)
// {
// 	typedef Domain::GetIdp2_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id(), name).get();
// }

// idm_t Domain_Client::GetIdm(idp_t idp)
// {
// 	typedef Domain::GetIdm1_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id(), idp).get();
// }

// idm_t Domain_Client::GetIdm(std::string const& name)
// {
// 	typedef Domain::GetIdm2_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id(), name).get();
// }

// idm_t Domain_Client::GetDynamicIdp()
// {
// 	typedef Domain::GetDynamicIdp_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id()).get();
// }




// /** Container interface **/
// std::string Domain_Client::GetGlobalContext()
// {
// 	typedef Domain::GetGlobalContext_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id()).get();
// }

// std::string Domain_Client::GetLocalContext()
// {
// 	typedef Domain::GetLocalContext_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id()).get();
// }

// unsigned int Domain_Client::GetTotalPods()
// {
// 	typedef Domain::GetTotalPods_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id()).get();
// }

// unsigned int Domain_Client::GetTotalDomains()
// {
// 	typedef Domain::GetTotalDomains_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id()).get();
// }

// idp_t Domain_Client::GetActiveResourceIdp()
// {
// 	auto id = hpx::threads::get_self_id();
// 	uint64_t* ptr=(uint64_t*) &id;
// 	auto ptr2 = (*ptr);
// 	// std::cout << "Thread HPX - GetActiveResourceIdp: " << ptr2 << std::endl;

// 	typedef cor::Domain::GetActiveResourceIdp_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id(), ptr2).get(); 
// }

// idp_t Domain_Client::GetPredecessorIdp(idp_t idp)
// {
// 	typedef cor::Domain::GetPredecessorIdp_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id(), idp).get(); 
// }

// template <typename T>
// std::unique_ptr<T> Domain_Client::GetLocalResource(idp_t idp)
// {
// 	typedef cor::Domain::GetLocalResource_action_Domain<T> action_type;
// 	return hpx::async<action_type>(this->get_id(), idp).get(); 
// }

// template <typename T, typename ... Args>
// std::unique_ptr<T> Domain_Client::CreateLocal(idp_t ctx, std::string const& name, Args&& ... args)
// {
// 	typedef cor::Domain::CreateLocal_action_Domain<T, Args...> action_type;
// 	return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
// }

// template <typename T, typename ... Args>
// idp_t Domain_Client::CreateRemote(idp_t ctx, std::string const& name, Args&& ... args)
// {
// 	typedef cor::Domain::CreateRemote_action_Domain<T, Args...> action_type;
// 	return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
// }

// template <typename T, typename ... Args>
// idp_t Domain_Client::Create(idp_t ctx, std::string const& name, Args&& ... args)
// {
// 	std::cout << "Domain_Client::Create" << std::endl;
// 	typedef cor::Domain::Create_action_Domain<T, Args...> action_type;
// 	return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
// }

// template <typename T>
// std::unique_ptr<T> Domain_Client::CreateReference(idp_t idp, idp_t ctx, std::string const& name)
// {
// 	typedef cor::Domain::CreateReference_action_Domain<T> action_type;
// 	return hpx::async<action_type>(this->get_id(), idp, ctx, name).get();
// }

// template <typename T, typename ... Args>
// std::unique_ptr<T> Domain_Client::CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args)
// {
// 	typedef cor::Domain::CreateCollective1_action_Domain<T, Args...> action_type;
// 	return hpx::async<action_type>(this->get_id(), ctx, name, total_members, std::forward<Args>(args)...).get();
// }

// template <typename T, typename ... Args>
// std::unique_ptr<T> Domain_Client::CreateCollective(idp_t clos, idp_t ctx, std::string const& name, Args&& ... args)
// {
// 	auto active_rsc_idp = GetActiveResourceIdp(); // vou buscar o idp do agente atual que vai ser necessario no Pod
// 	typedef cor::Domain::CreateCollective2_action_Domain<T, Args...> action_type;
// 	return hpx::async<action_type>(this->get_id(), active_rsc_idp, clos, ctx, name, std::forward<Args>(args)...).get();
// }

// template <typename T, typename ... Args>
// void Domain_Client::Run(idp_t idp, Args&&... args)
// {
// 	typedef cor::Domain::Run_action_Domain<T, Args...> action_type;
// 	return hpx::async<action_type>(this->get_id(), idp, std::forward<Args>(args)...).get();
// }

// template <typename T>
// void Domain_Client::Wait(idp_t idp)
// {
// 	typedef cor::Domain::Wait_action_Domain<T> action_type;
// 	return hpx::async<action_type>(this->get_id(), idp).get(); 
// }

// template <typename T>
// auto Domain_Client::Get(idp_t idp)
// {
// 	typedef cor::Domain::Get_action_Domain<T> action_type;
// 	return hpx::async<action_type>(this->get_id(), idp).get(); 
// }

// idp_t Domain_Client::Spawn(std::string const& context, unsigned int npods, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
// {
// 	auto parent = GetActiveResourceIdp();
// 	typedef cor::Domain::Spawn_action_Domain action_type;
// 	return hpx::async<action_type>(this->get_id(), context, npods, parent, module, args, hosts).get(); 
// }



// }
