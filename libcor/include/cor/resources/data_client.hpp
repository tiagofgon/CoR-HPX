#ifndef COR_DATA_CLIENT_HPP
#define COR_DATA_CLIENT_HPP

#include "cor/resources/data.hpp"
#include "cor/services/mutexRW_service_client.hpp"
#include "cor/services/resource_manager_global.hpp"
#include "hpx/modules/async_colocated.hpp"

#include <hpx/hpx.hpp>


// The front end for the data itself. Essentially wraps actions for
// the server, and stores information locally
namespace cor {

template <typename T>
class Data_Client : hpx::components::client_base<Data_Client<T>, Data<T>> 
{

public:
	typedef hpx::components::client_base<Data_Client<T>, Data<T>> base_type;

	friend class hpx::serialization::access;

	typedef nullptr_t organizer;

	// Default construct an empty client side representation (not
	// connected to any existing component). Also needed for serialization
    Data_Client()
    {}

	// Create a client side representation for the existing
	// Data instance with the given GID
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

	// Constructor for replicas
    Data_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(idp)
    {
		std::string basename = std::to_string(IdpGlobal()) + "Datamutex";
		hpx::id_type mutex_gid = hpx::find_from_basename(basename, 0).get();
		mutex = new MutexRWService_Client(std::move(mutex_gid));
	}

    Data_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(idp)
    {
		std::string basename = std::to_string(IdpGlobal()) + "Datamutex";
		hpx::id_type mutex_gid = hpx::find_from_basename(basename, 0).get();
		mutex = new MutexRWService_Client(std::move(mutex_gid));
	}

    Data_Client(idp_t idp, hpx::id_type && id) :
        base_type(std::move(id)),
		_idp(idp)
    {
		std::string basename = std::to_string(IdpGlobal()) + "Datamutex";
		hpx::id_type mutex_gid = hpx::find_from_basename(basename, 0).get();
		mutex = new MutexRWService_Client(std::move(mutex_gid));
	}

	// Standard constructor with parameters
	template <typename ... Args>
    Data_Client(idp_t idp, Args&&... args) :
        base_type(create_server(idp, std::forward<Args>(args)...)),
		_idp(idp)
    {
		// mutex necessary to guarantee mutual exclusion in data access
		mutex = new MutexRWService_Client();
		std::string basename = std::to_string(idp) + "Datamutex";
		auto future = hpx::register_with_basename(basename, mutex->GetGid(), 0).get();
	}

	template <typename ... Args>
    Data_Client(idp_t idp, hpx::id_type locality, Args&&... args) :
        base_type(create_server_remote(idp, locality, std::forward<Args>(args)...)),
		_idp(idp)
    {
		// Mmutex necessary to guarantee mutual exclusion in data access
		mutex = new MutexRWService_Client();
		std::string basename = std::to_string(idp) + "Datamutex";
		auto future = hpx::register_with_basename(basename, mutex->GetGid(), 0).get();
	}




	/** Resource's interface **/
	// method that returns the global idp of the resource, which is present in the class Resource
	hpx::future<idp_t> IdpGlobal(hpx::launch::async_policy)
	{
		typedef ResourceMigrable::Idp_action_ResourceMigrable action_type;
		return hpx::async<action_type>(base_type::get_id());
	}

	idp_t IdpGlobal()
	{
		typedef ResourceMigrable::Idp_action_ResourceMigrable action_type;
		return action_type()(base_type::get_id());
	}

	// method that returns the GID(hpx::id_type) of this resource locality
	hpx::future<hpx::id_type> GetLocalityGID(hpx::launch::async_policy)
	{
		typedef ResourceMigrable::GetLocalityGID_action_ResourceMigrable action_type;
		return hpx::async<action_type>(base_type::get_id());
	}

	hpx::id_type GetLocalityGID()
	{
		typedef ResourceMigrable::GetLocalityGID_action_ResourceMigrable action_type;
		return action_type()(base_type::get_id());
	}

	// method that returns the number of this resource locality
	hpx::future<unsigned int> GetLocalityID(hpx::launch::async_policy)
	{
		typedef ResourceMigrable::GetLocalityID_action_ResourceMigrable action_type;
		return hpx::async<action_type>(base_type::get_id());
	}

	unsigned int GetLocalityID()
	{
		typedef ResourceMigrable::GetLocalityID_action_ResourceMigrable action_type;
		return action_type()(base_type::get_id());	
	}




	/** Data's interface **/
	hpx::future<void> AcquireRead(hpx::launch::async_policy)
	{
		//std::cout << "AcquireRead()" << std::endl;
		return hpx::async([&](){
			mutex->AcquireRead();
		});
	}

	void AcquireRead()
	{
		//std::cout << "AcquireRead()" << std::endl;
		mutex->AcquireRead();
	}

	hpx::future<void> ReleaseRead(hpx::launch::async_policy)
	{
		//std::cout << "ReleaseRead()" << std::endl;
		return hpx::async([&](){
			mutex->ReleaseRead();
		});
	}

	void ReleaseRead()
	{
		//std::cout << "ReleaseRead()" << std::endl;
		mutex->ReleaseRead();
	}

	hpx::future<void> AcquireWrite(hpx::launch::async_policy)
	{
		//std::cout << "AcquireWrite()" << std::endl;
		return hpx::async([&](){
			mutex->AcquireWrite();
		});
	}

	void AcquireWrite()
	{
		//std::cout << "AcquireWrite()" << std::endl;
		mutex->AcquireWrite();
	}

	hpx::future<void> ReleaseWrite(hpx::launch::async_policy)
	{
		//std::cout << "ReleaseWrite()" << std::endl;
		return hpx::async([&](){
			mutex->ReleaseWrite();
		});
	}

	void ReleaseWrite()
	{
		//std::cout << "ReleaseWrite()" << std::endl;
		mutex->ReleaseWrite();
	}

    hpx::future<T> Fetch(hpx::launch::async_policy)
    {
		AcquireRead();
		typedef typename Data<T>::Fetch_action_Data action_type;
		auto dado = hpx::async<action_type>(base_type::get_id());
		ReleaseRead();
		return dado;
    }

    T Fetch()
    {
		AcquireRead();
		typedef typename Data<T>::Fetch_action_Data action_type;
		auto dado = action_type()(base_type::get_id());
		ReleaseRead();
		return dado;
    }

	T* Get()
	{
		ensure_ptr();
		return ptr->Get();
	}


    template <typename F, typename ... Args>
    auto Run(hpx::function<F> func, Args... args)
	{
		AcquireWrite();
		typedef typename Data<T>::template Run_action_Data<F, Args...> action_type;
		auto res = hpx::async<action_type>(base_type::get_id(), func, args...);
		ReleaseWrite();
		return res;
	}

    template <typename F, typename ... Args>
    auto Run(hpx::launch::sync_policy, hpx::function<F> func, Args... args)
	{
		AcquireWrite();
		typedef typename Data<T>::template Run_action_Data<F, Args...> action_type;
		auto res = action_type()(base_type::get_id(), func, args...);
		ReleaseWrite();
		return res;
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
		10 - Operon
		11 - UniChannel
		12 - MultiChannel
		*/
		return hpx::make_ready_future(6);
	}

	int GetComponentType()
	{
		return 6;
	}

	void Migrate(idp_t domain_target)
	{
		hpx::id_type resource_manager_global_component = hpx::find_from_basename("ResourceManagerGlobal_basename", 0).get();
		typedef ResourceManagerGlobal::GetGidFromIdp_action_ResourceManagerGlobal action_type;
		// get gid of domain_target
		hpx::id_type gid = hpx::async<action_type>(resource_manager_global_component, domain_target).get();

		auto locality = hpx::get_colocation_id(hpx::launch::sync, gid);

		ptr = nullptr;
		hpx::components::migrate<Data<T>>(this->get_id(), locality).get();
		//std::cout << "o objeto migrou para " << domain_target << std::endl;
		return;
	}

	// For compilation purposes only, it is never used here!
	hpx::future<hpx::id_type> GetMailboxGid(hpx::launch::async_policy) {
		return hpx::make_ready_future(hpx::find_here());
	}

	hpx::id_type GetMailboxGid() {
		return hpx::find_here();
	}
	



private:
	template <typename ... Args>
	hpx::future<hpx::id_type> create_server(idp_t idp, Args&&... args) {
		return hpx::local_new<Data<T>>(idp, std::forward<Args>(args)...);
	}
	template <typename ... Args>
	hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality, Args&&... args) {
		return hpx::new_<Data<T>>(locality, idp, std::forward<Args>(args)...);
	}

	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		ar & _idp;
	}

	idp_t _idp;
	MutexRWService_Client *mutex;
	mutable std::shared_ptr<Data<T>> ptr;

	hpx::future<hpx::id_type> Migrate_hpx(hpx::id_type dest)
	{
		ptr = nullptr;
		return hpx::components::migrate<Data<T>>(this->get_id(), dest);
	}
	
	void ensure_ptr() {
		// if the Data component is not in the current locality, migrate
		if(hpx::find_here() != hpx::get_colocation_id(hpx::launch::sync, this->get_id())) {

			//std::cout << "É preciso migrar" << std::endl;
			Migrate_hpx(hpx::find_here()).get();
			//std::cout << "Componente migrado" << std::endl;
			ptr = hpx::get_ptr<Data<T>>(hpx::launch::sync, this->get_id());
		}
		else {
			ptr = hpx::get_ptr<Data<T>>(hpx::launch::sync, this->get_id());
		}
	}


};

	
}


#endif
