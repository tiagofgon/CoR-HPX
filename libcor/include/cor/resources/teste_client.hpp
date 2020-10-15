
#ifndef COR_TESTE_CLIENT_HPP
#define COR_TESTE_CLIENT_HPP

#include <hpx/hpx.hpp>

#include "cor/resources/teste.hpp"

namespace cor {

class Teste_Client : hpx::components::client_base<Teste_Client, Teste>
{

private:
	static hpx::future<hpx::id_type> create_server(idp_t idp, std::string const& module) {
		return hpx::local_new<Teste>(idp, module);
	}
	static hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, std::string const& module) {
		return hpx::new_<Teste>(locality, idp, module);
	}

	typedef typename Teste::data_type data_type;

public:
	typedef hpx::components::client_base<Teste_Client, Teste> base_type;

	friend class hpx::serialization::access;
	friend class Teste;

	/// Default construct an empty client side representation (not
	/// connected to any existing component). Also needed for serialization
	Teste_Client()
	{}

	/// Create a client side representation for the existing
	/// Closure instance with the given GID
	Teste_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Teste_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Teste_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	// Construtor para réplicas
	Teste_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Teste_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Teste_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	/// Standard constructor with parameters
	Teste_Client(idp_t idp, std::string const& module) :
		base_type(create_server(idp, module)),
		_idp(idp)
	{
		global::pod->setDomainIdp(idp); // Update Teste idp variable's Pod
	}

	// Isto só é possivel fazer quando se implementar dominios remotos!
	Teste_Client(idp_t idp, hpx::id_type locality, std::string const& module) :
		base_type(create_server_remote(idp, locality, module)),
		_idp(idp)
	{
		global::pod->setDomainIdp(idp); // Update Teste idp variable's Pod
	}


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


	/** Dynamic organizer interface **/
	void Join(idp_t idp, std::string const& name)
	{
		typedef Teste::Join_action_Teste action_type;
		return hpx::async<action_type>(this->get_id(), idp, name).get();
	}

	void Leave(idp_t idp)
	{
		typedef Teste::Leave_action_Teste action_type;
		return hpx::async<action_type>(this->get_id(), idp).get();
	}
	
	std::string GetModuleName()
	{
		typedef Teste::GetModuleName_action_Teste action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}
	
	std::size_t GetTotalMembers()
	{
		typedef Teste::GetTotalMembers_action_Teste action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	std::vector<idp_t> GetMemberList()
	{
		typedef Teste::GetMemberList_action_Teste action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

    idp_t GetIdp(idm_t idm)
	{
	  typedef Teste::GetIdp1_action_Teste action_type;
	  return hpx::async<action_type>(this->get_id(), idm).get();
	}

    idp_t GetIdp(std::string const& name)
	{
	  typedef Teste::GetIdp2_action_Teste action_type;
	  return hpx::async<action_type>(this->get_id(), name).get();
	}

    idm_t GetIdm(idp_t idp)
	{
	  typedef Teste::GetIdm1_action_Teste action_type;
	  return hpx::async<action_type>(this->get_id(), idp).get();
	}

    idm_t GetIdm(std::string const& name)
	{
	  typedef Teste::GetIdm2_action_Teste action_type;
	  return hpx::async<action_type>(this->get_id(), name).get();
	}

    idm_t GetDynamicIdp()
	{
	  typedef Teste::GetDynamicIdp_action_Teste action_type;
	  return hpx::async<action_type>(this->get_id()).get();
	}
	




	/** Container interface **/
    std::string GetGlobalContext()
	{
		typedef Teste::GetGlobalContext_action_Teste action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

    std::string GetLocalContext()
	{
		typedef Teste::GetLocalContext_action_Teste action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

    unsigned int GetTotalPods()
	{
		typedef Teste::GetTotalPods_action_Teste action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}
	
    unsigned int GetTotalDomains()
	{
		typedef Teste::GetTotalDomains_action_Teste action_type;
		return hpx::async<action_type>(this->get_id()).get();
	}

	idp_t GetActiveResourceIdp()
	{
		auto id = hpx::threads::get_self_id();
		uint64_t* ptr=(uint64_t*) &id;
		auto ptr2 = (*ptr);
		// std::cout << "Thread HPX - GetActiveResourceIdp: " << ptr2 << std::endl;

		typedef cor::Teste::GetActiveResourceIdp_action_Teste action_type;
		return hpx::async<action_type>(this->get_id(), ptr2).get(); 
	}

	idp_t GetPredecessorIdp(idp_t idp)
	{
		typedef cor::Teste::GetPredecessorIdp_action_Teste action_type;
		return hpx::async<action_type>(this->get_id(), idp).get(); 
	}

	template <typename T>
	std::unique_ptr<T> GetLocalResource(idp_t idp)
	{
		typedef cor::Teste::GetLocalResource_action_Teste<T> action_type;
		return hpx::async<action_type>(this->get_id(), idp).get(); 
	}

	template <typename T, typename ... Args>
	std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, Args&& ... args)
	{

    // if(hpx::get_locality_id()==0) {
    //     std::vector<hpx::id_type> localities = hpx::find_remote_localities();
        
    //     std::cout << "Minha localidade: " << hpx::find_here() << std::endl;

    //     auto id_type = hpx::new_<Teste>(hpx::find_here()).get();

    //     for (hpx::id_type const& id : localities)
    //     {
    //         std::cout << "Localidade remota: " << id << std::endl;
    //         // std::cout << "O Dado está na localidade: " << data->GetLocalityGID() << std::endl;
    //         auto newgid = hpx::components::migrate<Teste>(id_type, id).get();

    //         std::cout << "O Dado está na localidade: " << hpx::get_colocation_id(hpx::launch::sync, newgid) << std::endl;

    //     }
    // }


		typedef cor::Teste::CreateLocal_action_Teste<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
	}


	void Tiago()
	{

	// 	std::cout << "Minha localidade antes de entrar no if: " << hpx::find_here() << std::endl;
    // // if(hpx::get_locality_id()==0) {
    //     std::vector<hpx::id_type> localities = hpx::find_remote_localities();
        
    //     std::cout << "Minha localidade: " << hpx::find_here() << std::endl;

    //     auto id_type = hpx::new_<Teste>(hpx::find_here()).get();

    //     for (hpx::id_type const& id : localities)
    //     {
    //         std::cout << "Localidade remota: " << id << std::endl;
    //         // std::cout << "O Dado está na localidade: " << data->GetLocalityGID() << std::endl;
    //         auto newgid = hpx::components::migrate<Teste>(id_type, id).get();

    //         std::cout << "O Dado está na localidade: " << hpx::get_colocation_id(hpx::launch::sync, newgid) << std::endl;

    //     }
    // }

	}


    template <typename T, typename ... Args>
    idp_t CreateRemote(idp_t ctx, std::string const& name, Args&& ... args)
	{
		typedef cor::Teste::CreateRemote_action_Teste<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
	}

    template <typename T, typename ... Args>
    idp_t Create(idp_t ctx, std::string const& name, Args&& ... args)
	{
		typedef cor::Teste::Create_action_Teste<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, std::forward<Args>(args)...).get();
	}

	template <typename T, typename ... Args>
	std::unique_ptr<T> CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args)
	{
		typedef cor::Teste::CreateCollective1_action_Teste<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), ctx, name, total_members, std::forward<Args>(args)...).get();
	}

	// template <typename T, typename ... Args>
	// std::unique_ptr<T> CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args& ... args)
	// {
	// 	typedef cor::Teste::CreateCollective1_action_Teste<T, Args...> action_type;
	// 	return hpx::async<action_type>(this->get_id(), ctx, name, total_members, (args)...).get();
	// }

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective(idp_t clos, idp_t ctx, std::string const& name, Args&& ... args)
	{


    // if(hpx::get_locality_id()==0) {
    //     std::vector<hpx::id_type> localities = hpx::find_remote_localities();
        
    //     std::cout << "Minha localidade: " << hpx::find_here() << std::endl;

    //     auto id_type = hpx::new_<Teste>(hpx::find_here()).get();

    //     for (hpx::id_type const& id : localities)
    //     {
    //         std::cout << "Localidade remota: " << id << std::endl;
    //         // std::cout << "O Dado está na localidade: " << data->GetLocalityGID() << std::endl;
    //         auto newgid = hpx::components::migrate<Teste>(id_type, id).get();

    //         std::cout << "O Dado está na localidade: " << hpx::get_colocation_id(hpx::launch::sync, newgid) << std::endl;

    //     }
    // }


		auto active_rsc_idp = GetActiveResourceIdp(); // vou buscar o idp do agente atual que vai ser necessario no Pod
		typedef cor::Teste::CreateCollective2_action_Teste<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), active_rsc_idp, clos, ctx, name, std::forward<Args>(args)...).get();
	}

    template <typename T, typename ... Args>
    void Run(idp_t idp, Args&&... args)
	{
		typedef cor::Teste::Run_action_Teste<T, Args...> action_type;
		return hpx::async<action_type>(this->get_id(), idp, std::forward<Args>(args)...).get();
	}

    template <typename T>
    void Wait()
	{
		typedef cor::Teste::Wait_action_Teste<T> action_type;
		return hpx::async<action_type>(this->get_id()).get(); 
	}

    template <typename T>
    auto Get()
	{
		typedef cor::Teste::Get_action_Teste<T> action_type;
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

	std::vector<std::string> GetComponentHierarchy()
	{
		std::vector<std::string> str = {"Teste", "Resource", "DynamicOrganizer", "Container"};
		return str;
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