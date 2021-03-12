
#ifndef COR_MUTEX_SERVICE_CLIENT_HPP
#define COR_MUTEX_SERVICE_CLIENT_HPP

#include <hpx/hpx.hpp>

#include "cor/services/mutex_service.hpp"


namespace cor {

struct MutexService_Client: hpx::components::client_base<MutexService_Client, MutexService>
{

private:
	static hpx::future<hpx::id_type> create_server() {
		return hpx::local_new<MutexService>();
	}

public:
	typedef hpx::components::client_base<MutexService_Client, MutexService> base_type;

	friend class hpx::serialization::access;

	/// Default construct an empty client side representation (not
	/// connected to any existing component). Also needed for serialization
	MutexService_Client() :
		base_type(create_server())
	{}

	/// Create a client side representation for the existing
	/// Closure_Component instance with the given GID
	MutexService_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id))
	{}

	MutexService_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id))
	{}

	MutexService_Client(hpx::id_type && id) :
		base_type(std::move(id))
	{}


	/** MutexService interface **/
    void Acquire()
	{
	  typedef MutexService::Acquire_action_MutexService action_type;
	  return hpx::async<action_type>(this->get_id()).get();
	}

    void Release()
	{
	  typedef MutexService::Release_action_MutexService action_type;
	  return hpx::async<action_type>(this->get_id()).get();
	}


	// Returns component's GID
	hpx::id_type GetGid() {
	  return this->get_id();
	}

  private:
	template <typename Archive>
	void serialize(Archive& ar, unsigned) {   
		// std::cout << "serialized\n";
	}

};


}

#endif