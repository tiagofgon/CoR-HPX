
#ifndef COR_DATA_CLIENT_HPP
#define COR_DATA_CLIENT_HPP

#include "cor/resources/data.hpp"
#include "cor/services/mutex_service_client.hpp"
// #include "cor/services/mutex_service.hpp"
// #include "cor/resources/teste.hpp"


#include <hpx/hpx.hpp>


// The front end for the data itself. Essentially wraps actions for
// the server, and stores information locally about the localities/servers
// that it needs to know about
namespace cor {

// template <typename> class Data_Client;

template <typename T>
class Data_Client : hpx::components::client_base<Data_Client<T>, Data<T>> 
{

private:
	template <typename ... Args>
	static hpx::future<hpx::id_type> create_server(idp_t idp, Args&&... args) {
		return hpx::local_new<Data<T>>(idp, std::forward<Args>(args)...);
	}
	template <typename ... Args>
	static hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, Args&&... args) {
		return hpx::new_<Data<T>>(locality, idp, std::forward<Args>(args)...);
	}

public:
	typedef hpx::components::client_base<Data_Client<T>, Data<T>> base_type;

	friend class hpx::serialization::access;

	/// Default construct an empty client side representation (not
	/// connected to any existing component). Also needed for serialization
    Data_Client()
    {}

	/// Create a client side representation for the existing
	/// Data instance with the given GID
    Data_Client(hpx::future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(IdpGlobal())
    {}

    Data_Client(hpx::shared_future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(IdpGlobal())
    {}

    Data_Client(hpx::id_type && id) :
        base_type(std::move(id)),
		_idp(IdpGlobal())
    {}

	// Construtor para réplicas
    Data_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(idp)
    {
		std::string basename = std::to_string(IdpGlobal()) + "mutex";
		hpx::id_type mutex_gid = hpx::find_from_basename(basename, 0).get();
		mutex = new MutexService_Client(std::move(mutex_gid));
	}

    Data_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(idp)
    {
		std::string basename = std::to_string(IdpGlobal()) + "mutex";
		hpx::id_type mutex_gid = hpx::find_from_basename(basename, 0).get();
		mutex = new MutexService_Client(std::move(mutex_gid));
	}

    Data_Client(idp_t idp, hpx::id_type && id) :
        base_type(std::move(id)),
		_idp(idp)
    {
		std::string basename = std::to_string(IdpGlobal()) + "mutex";
		hpx::id_type mutex_gid = hpx::find_from_basename(basename, 0).get();
		mutex = new MutexService_Client(std::move(mutex_gid));
	}

	/// Standard constructor with parameters
	template <typename ... Args>
    Data_Client(idp_t idp, Args&&... args) :
        base_type(create_server(idp, std::forward<Args>(args)...)),
		_idp(idp)
    {
		// Mutex necessário para garantir exclusão mutua no acesso ao dado
		mutex = new MutexService_Client();
		std::string basename = std::to_string(idp) + "mutex";
		auto future = hpx::register_with_basename(basename, mutex->GetGid()).get();

	}

	template <typename ... Args>
    Data_Client(idp_t idp, hpx::id_type locality, Args&&... args) :
        base_type(create_server_remote(idp, locality, std::forward<Args>(args)...)),
		_idp(idp)
    {
		// Mutex necessário para garantir exclusão mutua no acesso ao dado
		mutex = new MutexService_Client();
		std::string basename = std::to_string(idp) + "mutex";
		auto future = hpx::register_with_basename(basename, mutex->GetGid()).get();
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


	/** Data interface **/
    T Fetch()
    {
      typedef typename Data<T>::Fetch_action_Data action_type;
      return hpx::async<action_type>(base_type::get_id()).get();
    }

	void Acquire()
	{
		std::cout << "Acquire()" << std::endl;

		// // typedef typename Data<T>::Acquire_action_Data action_type;
		// // int state = hpx::async<action_type>(base_type::get_id()).get();
		mutex->Acquire();

		// // mutex_gid = hpx::local_new<MutexService>().get();
		// // std::cout << "Acquire()11" << std::endl;
		// // typedef MutexService::Acquire_action_MutexService action_type;
		// // std::cout << "Acquire()22" << std::endl;
		// // hpx::async<action_type>(mutex_gid).get();
		// // std::cout << "Acquire()33" << std::endl;
		ensure_ptr();


		// // typedef MutexService::Release_action_MutexService action_type2;
		// // std::cout << "Release()22" << std::endl;
		// // hpx::async<action_type2>(mutex_gid).get();
		// // std::cout << "Release()33" << std::endl;
	}

	void Release()
	{
		std::cout << "Release()" << std::endl;
		// // typedef typename Data<T>::Release_action_Data action_type;
		// // return hpx::async<action_type>(base_type::get_id()).get();
		mutex->Release();
		// // typedef MutexService::Release_action_MutexService action_type;
		// // hpx::async<action_type>(mutex_gid).get();
	}



	T const &operator*() const {
		std::cout << "AQUI_11" << std::endl;
		HPX_ASSERT(this->get_id());
		// ensure_ptr();
		
		return **ptr;
	}

	T &operator*() {
		std::cout << "AQUI_22" << std::endl;
		HPX_ASSERT(this->get_id());
		// ensure_ptr();
		
		return **ptr;
	}

	T const* operator->() const
	{
		std::cout << "AQUI_33" << std::endl;
		HPX_ASSERT(this->get_id());
		// ensure_ptr();
		
		return &**ptr;
	}

	T* operator->()
	{
		std::cout << "AQUI_44" << std::endl;
		HPX_ASSERT(this->get_id());
		// ensure_ptr();
		
		return &**ptr;
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
		std::vector<std::string> str = {"Data", "Resource", "Value"};
		return str;
	}

	void Migrate(hpx::id_type dest)
	{
		hpx::components::migrate<Data<T>>(this->get_id(), dest).get();
	}

private:
	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
		// std::cout << "serialized\n";
	}

	idp_t _idp;

	MutexService_Client *mutex;
	// hpx::id_type mutex_gid;

	mutable std::shared_ptr<Data<T>> ptr;
	void ensure_ptr() {

		// std::cout << "GID1: " << this->get_id() << std::endl;
		// std::cout << "locality1: " << hpx::get_locality_id() << std::endl;

		// Se o componente Data não estiver na localidade atual, faz o migrate
		if(hpx::find_here() != hpx::get_colocation_id(hpx::launch::sync, this->get_id())) {

			std::cout << "É preciso migrar" << std::endl;
			// auto equal_gid = hpx::components::migrate<Data<T>>(this->get_id(), hpx::find_here()).get();
			Migrate(hpx::find_here());
			std::cout << "Componente migrado" << std::endl;
			ptr = hpx::get_ptr<Data<T>>(hpx::launch::sync, this->get_id());
		}
		else {
			ptr = hpx::get_ptr<Data<T>>(hpx::launch::sync, this->get_id());
		}
		// std::cout << "locality2: " << hpx::get_locality_id() << std::endl;
		// std::cout << "GID2: " << equal_gid << std::endl;
		// std::cout << "GID3: " << this->get_id() << std::endl;

		// std::cout << "AQUI_ensure_ptr" << std::endl;
		// if (!ptr) {
		// 	ptr = hpx::get_ptr<Data<T>>(hpx::launch::sync, this->get_id());
		// }
	}

};

	
}

#endif
