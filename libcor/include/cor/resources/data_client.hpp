#ifndef COR_DATA_CLIENT_HPP
#define COR_DATA_CLIENT_HPP

#include "cor/resources/data.hpp"
#include "cor/services/mutexRW_service_client.hpp"

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
		_idp(IdpGlobal().get())
    {}

    Data_Client(hpx::shared_future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(IdpGlobal().get())
    {}

    Data_Client(hpx::id_type && id) :
        base_type(std::move(id)),
		_idp(IdpGlobal().get())
    {}

	// Constructor for replicas
    Data_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(idp)
    {
		std::string basename = std::to_string(IdpGlobal().get()) + "Datamutex";
		hpx::id_type mutex_gid = hpx::find_from_basename(basename, 0).get();
		mutex = new MutexRWService_Client(std::move(mutex_gid));
	}

    Data_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
        base_type(std::move(id)),
		_idp(idp)
    {
		std::string basename = std::to_string(IdpGlobal().get()) + "Datamutex";
		hpx::id_type mutex_gid = hpx::find_from_basename(basename, 0).get();
		mutex = new MutexRWService_Client(std::move(mutex_gid));
	}

    Data_Client(idp_t idp, hpx::id_type && id) :
        base_type(std::move(id)),
		_idp(idp)
    {
		std::string basename = std::to_string(IdpGlobal().get()) + "Datamutex";
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
		auto future = hpx::register_with_basename(basename, mutex->GetGid()).get();
	}

	template <typename ... Args>
    Data_Client(idp_t idp, hpx::id_type locality, Args&&... args) :
        base_type(create_server_remote(idp, locality, std::forward<Args>(args)...)),
		_idp(idp)
    {
		// Mmutex necessary to guarantee mutual exclusion in data access
		mutex = new MutexRWService_Client();
		std::string basename = std::to_string(idp) + "Datamutex";
		auto future = hpx::register_with_basename(basename, mutex->GetGid()).get();
	}


	/** Resource's interface **/
	// method that returns the global idp of the resource, which is present in the class Resource
    hpx::future<idp_t> IdpGlobal()
    {
		typedef ResourceMigrable::Idp_action_ResourceMigrable action_type;
		return hpx::async<action_type>(base_type::get_id());
    }

    hpx::future<idp_t> IdpGlobal_here()
    {
		Migrate(hpx::find_here());
		typedef ResourceMigrable::Idp_action_ResourceMigrable action_type;
		return hpx::async<action_type>(base_type::get_id());
    }

	// method that returns the GID(hpx::id_type) of this resource locality
	hpx::future<hpx::id_type> GetLocalityGID()
	{
		typedef ResourceMigrable::GetLocalityGID_action_ResourceMigrable action_type;
		return hpx::async<action_type>(base_type::get_id());
	}
	
	hpx::future<hpx::id_type> GetLocalityGID_here()
	{
		Migrate(hpx::find_here());
		typedef ResourceMigrable::GetLocalityGID_action_ResourceMigrable action_type;
		return hpx::async<action_type>(base_type::get_id());
	}

	// method that returns the number of this resource locality
	hpx::future<unsigned int> GetLocalityID()
	{
		typedef ResourceMigrable::GetLocalityID_action_ResourceMigrable action_type;
		return hpx::async<action_type>(base_type::get_id());
	}

	hpx::future<unsigned int> GetLocalityID_here()
	{
		Migrate(hpx::find_here());
		typedef ResourceMigrable::GetLocalityID_action_ResourceMigrable action_type;
		return hpx::async<action_type>(base_type::get_id());
	}


	/** Data's interface **/
	hpx::future<void> AcquireRead()
	{
		//std::cout << "AcquireRead()" << std::endl;
		mutex->AcquireRead();
		ensure_ptr();
		return hpx::make_ready_future();
	}

	hpx::future<void> ReleaseRead()
	{
		//std::cout << "ReleaseRead()" << std::endl;
		mutex->ReleaseRead();
		return hpx::make_ready_future();
	}

	hpx::future<void> Acquire()
	{
		//std::cout << "AcquireWrite()" << std::endl;
		mutex->AcquireWrite();
		ensure_ptr();
		return hpx::make_ready_future();
	}

	hpx::future<void> Release()
	{
		//std::cout << "ReleaseWrite()" << std::endl;
		mutex->ReleaseWrite();
		return hpx::make_ready_future();
	}

    hpx::future<T> Fetch()
    {
		AcquireRead();
		typedef typename Data<T>::Fetch_action_Data action_type;
		auto dado = hpx::async<action_type>(base_type::get_id());
		ReleaseRead();
		return dado;
    }

	T* Get()
	{
		return ptr->Get();
	}

	T const &operator*() const {
		// std::cout << "AQUI_11" << std::endl;
		HPX_ASSERT(this->get_id());
		
		return **ptr;
	}

	T &operator*() {
		// std::cout << "AQUI_22" << std::endl;
		HPX_ASSERT(this->get_id());
		
		return **ptr;
	}

	T const* operator->() const
	{
		// std::cout << "AQUI_33" << std::endl;
		HPX_ASSERT(this->get_id());
		
		return &**ptr;
	}

	T* operator->()
	{
		// std::cout << "AQUI_44" << std::endl;
		HPX_ASSERT(this->get_id());
		
		return &**ptr;
	}


	/** Local Client's interface **/
	// local idp of this resource
	hpx::future<idp_t> Idp() {
		return hpx::make_ready_future(_idp);
	}

	// Returns component's GID
    hpx::future<hpx::id_type> GetGid() {
      return hpx::make_ready_future(this->get_id());
    }

	hpx::future<int> GetComponentType()
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
		return hpx::make_ready_future(6);
	}

	hpx::future<void> Migrate(hpx::id_type dest)
	{
		hpx::components::migrate<Data<T>>(this->get_id(), dest);
	}

	// For compilation purposes only, it is never used here!
	hpx::future<hpx::id_type> GetMailboxGid() {
		return hpx::make_ready_future(hpx::find_here());
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

	void ensure_ptr() {
		// if the Data component is not in the current locality, migrate
		if(hpx::find_here() != hpx::get_colocation_id(hpx::launch::sync, this->get_id())) {

			std::cout << "É preciso migrar" << std::endl;
			Migrate(hpx::find_here());
			std::cout << "Componente migrado" << std::endl;
			ptr = hpx::get_ptr<Data<T>>(hpx::launch::sync, this->get_id());
		}
		else {
			ptr = hpx::get_ptr<Data<T>>(hpx::launch::sync, this->get_id());
		}
	}


};

	
}


#endif
