
#ifndef COR_MUTEXRW_SERVICE_CLIENT_HPP
#define COR_MUTEXRW_SERVICE_CLIENT_HPP

#include <hpx/hpx.hpp>

#include "cor/services/mutexRW_service.hpp"


namespace cor {

struct MutexRWService_Client: hpx::components::client_base<MutexRWService_Client, MutexRWService>
{

private:
	static hpx::future<hpx::id_type> create_server() {
		return hpx::local_new<MutexRWService>();
	}

public:
	typedef hpx::components::client_base<MutexRWService_Client, MutexRWService> base_type;

	friend class hpx::serialization::access;

	/// Default construct an empty client side representation (not
	/// connected to any existing component). Also needed for serialization
	MutexRWService_Client() :
		base_type(create_server())
	{}

	/// Create a client side representation for the existing
	/// Closure_Component instance with the given GID
	MutexRWService_Client(hpx::future<hpx::id_type> && id) :
		base_type(std::move(id))
	{}

	MutexRWService_Client(hpx::shared_future<hpx::id_type> && id) :
		base_type(std::move(id))
	{}

	MutexRWService_Client(hpx::id_type && id) :
		base_type(std::move(id))
	{}


	/** MutexRWService interface **/
    void AcquireRead()
	{
	  typedef MutexRWService::AcquireRead_action_MutexRWService action_type;
	  return hpx::async<action_type>(this->get_id()).get();
	}

    void ReleaseRead()
	{
	  typedef MutexRWService::ReleaseRead_action_MutexRWService action_type;
	  return hpx::async<action_type>(this->get_id()).get();
	}

    void AcquireWrite()
	{
	  typedef MutexRWService::AcquireWrite_action_MutexRWService action_type;
	  return hpx::async<action_type>(this->get_id()).get();
	}

    void ReleaseWrite()
	{
	  typedef MutexRWService::ReleaseWrite_action_MutexRWService action_type;
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