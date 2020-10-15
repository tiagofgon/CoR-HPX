
#ifndef COR_MUTEX_CLIENT_HPP
#define COR_MUTEX_CLIENT_HPP

#include <hpx/hpx.hpp>

#include "cor/resources/mutex.hpp"


namespace cor {

struct Mutex_Client: hpx::components::client_base<Mutex_Client, Mutex>
{

private:
	static hpx::future<hpx::id_type> create_server(idp_t idp) {
		return hpx::local_new<Mutex>(idp);
	}
	static hpx::future<hpx::id_type> create_server_remote(idp_t idp, hpx::id_type locality) {
		return hpx::new_<Mutex>(locality, idp);
	}

public:
	typedef hpx::components::client_base<Mutex_Client, Mutex> base_type;

	friend class hpx::serialization::access;

	typedef nullptr_t organizer;

	/// Default construct an empty client side representation (not
	/// connected to any existing component). Also needed for serialization
	Mutex_Client()
	{}

	/// Create a client side representation for the existing
	/// Closure_Component instance with the given GID
	Mutex_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Mutex_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	Mutex_Client(hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(IdpGlobal())
	{}

	// Construtor para réplicas
	Mutex_Client(idp_t idp, hpx::future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Mutex_Client(idp_t idp, hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	Mutex_Client(idp_t idp, hpx::id_type && id) :
		base_type(std::move(id)),
		_idp(idp)
	{}

	/// Standard constructor with parameters
	Mutex_Client(idp_t idp) :
		base_type(create_server(idp)),
		_idp(idp)
	{}

	Mutex_Client(idp_t idp, hpx::id_type locality) :
		base_type(create_server_remote(idp, locality)),
		_idp(idp)
	{}


	/** ResourceNonMigrable interface **/
	// método que retorna o idp global do recurso, que está presente na classe ResourceNonMigrable
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
	

	/** Mutex interface **/
    void Acquire(std::string const& name)
	{
	  typedef Mutex::Acquire_action_Mutex action_type;
	  return hpx::async<action_type>(this->get_id()).get();
	}

    void Release()
	{
	  typedef Mutex::Release_action_Mutex action_type;
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

	// void Migrate(hpx::id_type dest)
	// {
	// 	hpx::components::migrate<Mutex>(this->get_id(), dest).get();
	// }

	std::vector<std::string> GetComponentHierarchy()
	{
		std::vector<std::string> str = {"Mutex", "Resource"};
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