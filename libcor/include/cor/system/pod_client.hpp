
#ifndef COR_POD_CLIENT_HPP
#define COR_POD_CLIENT_HPP



#include <hpx/hpx.hpp>

#include "cor/system/pod.hpp"

#include "cor/resources/closure_client.hpp"

namespace cor {


class Pod_Client : hpx::components::client_base<Pod_Client, Pod>
{

private:
	static hpx::future<hpx::id_type> create_server(std::string const& id, std::string const& app_group, std::string const& context, unsigned int npods) {
		return hpx::local_new<Pod>(id, app_group, context, npods);
	}

public:
    typedef hpx::components::client_base<Pod_Client, Pod> base_type;

    /// Default construct an empty client side representation (not
    /// connected to any existing component).
    Pod_Client()
    {}

    /// Create a client side representation for the existing
    /// Agent_Component instance with the given GID.
    Pod_Client(hpx::future<hpx::id_type> && id) :
        base_type(std::move(id))
    {}

    Pod_Client(hpx::shared_future<hpx::id_type> && id) :
        base_type(std::move(id))
    {}

    Pod_Client(hpx::id_type && id) :
        base_type(std::move(id))
    {}

	/// Standard constructor with parameters
	Pod_Client(std::string const& id, std::string const& app_group, std::string const& context, unsigned int npods) :
		base_type(create_server(id, app_group, context, npods))
	{}

    void Initialize()
    {
        typedef cor::Pod::Initialize_action_pod action_type;
        return hpx::async<action_type>(this->get_id()).get();
    }

    void Finalize()
    {
        typedef cor::Pod::Finalize_action_pod action_type;
        return hpx::async<action_type>(this->get_id()).get(); 
    }

    std::string GetGlobalContext()
    {
        typedef cor::Pod::GetGlobalContext_action_pod action_type;
        return hpx::async<action_type>(this->get_id()).get(); 
    }

    std::string GetLocalContext()
    {
        typedef cor::Pod::GetLocalContext_action_pod action_type;
        return hpx::async<action_type>(this->get_id()).get(); 
    }

	unsigned int GetNumPods()
	{
		typedef cor::Pod::GetNumPods_action_pod action_type;
		return hpx::async<action_type>(this->get_id()).get(); 
	}

	unsigned int GetNumDomains()
	{
		typedef cor::Pod::GetNumDomains_action_pod action_type;
		return hpx::async<action_type>(this->get_id()).get(); 
	}

	std::vector<idp_t> GetPods()
	{
		typedef cor::Pod::GetPods_action_pod action_type;
		return hpx::async<action_type>(this->get_id()).get(); 
	}

	std::vector<idp_t> GetDomains()
	{
		typedef cor::Pod::GetDomains_action_pod action_type;
		return hpx::async<action_type>(this->get_id()).get(); 
	}


    // método que irá ser invocado internamente
    idp_t GetActiveResourceIdp()
    {
        auto id = hpx::threads::get_self_id();
        uint64_t* ptr=(uint64_t*) &id;
        auto idd = (*ptr);

        typedef cor::Pod::GetActiveResourceIdp_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), idd).get(); 
    }

    // método que irá ser invocado através do Domain(container)
    idp_t GetActiveResourceIdp2(size_t id)
    {
        typedef cor::Pod::GetActiveResourceIdp_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), id).get(); 
    }

    idp_t GetDomainIdp()
    {
        typedef cor::Pod::GetDomainIdp1_action_pod action_type;
        return hpx::async<action_type>(this->get_id()).get(); 
    }

    idp_t GetDomainIdp(idp_t idp)
    {
        typedef cor::Pod::GetDomainIdp2_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), idp).get(); 
    }

    void setDomainIdp(idp_t idp)
    {
        typedef cor::Pod::setDomainIdp_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), idp).get(); 
    }

    idp_t GetPredecessorIdp(idp_t idp)
    {
        typedef cor::Pod::GetPredecessorIdp_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), idp).get(); 
    }

    hpx::id_type GetGidFromIdp(idp_t idp)
    {
        typedef cor::Pod::GetGidFromIdp_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), idp).get(); 
    }

    template <typename T>
    std::unique_ptr<T> GetLocalResource(idp_t idp)
    {
        typedef cor::Pod::GetLocalResource_action_pod<T> action_type;
        return hpx::async<action_type>(this->get_id(), idp).get(); 
    }

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateLocal_test(idp_t ctx, std::string const& name, Args ... args)
    {
        typedef cor::Pod::CreateLocal_test_action_pod<T, Args...> action_type;
        return hpx::async<action_type>(this->get_id(), ctx, name, args...).get();
    }

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, Args ... args)
    {
		std::shared_ptr<Pod> ptr = hpx::get_ptr<Pod>(hpx::launch::sync, this->get_id());
		std::unique_ptr<T> res = ptr->CreateLocal<T, Args...>(ctx, name, args...);
        return res;
    }

    template <typename T, typename ... Args>
    idp_t Create(idp_t ctx, std::string const& name, Args ... args)
    {
        typedef cor::Pod::Create_action_pod<T, Args...> action_type;
        return hpx::async<action_type>(this->get_id(), ctx, name, args...).get();
    }

    template <typename T, typename ... Args>
    idp_t CreateRemote(idp_t ctx, std::string const& name, std::string const& ctrl, Args ... args)
    {
        typedef cor::Pod::CreateRemote_action_pod<T, Args...> action_type;
        return hpx::async<action_type>(this->get_id(), ctx, name, ctrl, args...).get();
    }

    template <typename T>
    std::unique_ptr<T> CreateReference(idp_t idp, idp_t ctx, std::string const& name)
	{
		typedef cor::Pod::CreateReference_action_pod<T> action_type;
		return hpx::async<action_type>(this->get_id(), idp, ctx, name).get();
	}

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args ... args)
    {
        typedef cor::Pod::CreateCollective1_action_pod<T, Args...> action_type;
        return hpx::async<action_type>(this->get_id(), ctx, name, total_members, args...).get();
    }

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective(idp_t agent_idp, idp_t comm, idp_t ctx, std::string const& name, Args ... args)
    {
        auto active_rsc_idp = agent_idp;
        auto sorg = GetLocalResource<cor::Closure_Client>(comm); // vou buscar a clausura identificada por comm
        auto rank = sorg->GetIdm(active_rsc_idp); // vou buscar o idm do agente atual, idm que pertence à clausura

        typedef cor::Pod::CreateCollective2_action_pod<T, Args...> action_type;
        return hpx::async<action_type>(this->get_id(), rank, comm, ctx, name, args...).get();
    }
    

	idp_t Spawn(std::string const& context, unsigned int npods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
	{
		typedef cor::Pod::Spawn_action_pod action_type2;
		return hpx::async<action_type2>(this->get_id(), context, npods, parent, module, args, hosts).get(); 
	}
    
    std::string SearchResource(idp_t idp)
    {
        typedef cor::Pod::SearchResource_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), idp).get(); 
    }
    
    bool ContainsResource(idp_t idp)
    {
        typedef cor::Pod::ContainsResource_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), idp).get(); 
    }
    
    void LoadModule(std::string const& module)
    {
        typedef cor::Pod::LoadModule_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), module).get(); 
    }

    template <typename T>
    std::function<T> LoadFunction(std::string const& module, std::string const& function)
    {
        typedef cor::Pod::LoadFunction_action_pod<T> action_type;
        return hpx::async<action_type>(this->get_id(), module, function).get(); 
    }

    void InsertActiveResource(size_t tid, idp_t idp)
    {
        typedef cor::Pod::InsertActiveResource_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), tid, idp).get();
    }

    void RemoveActiveResource(size_t tid)
    {
        typedef cor::Pod::RemoveActiveResource_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), tid).get(); 
    }

    void ChangeActiveResource(size_t tid, idp_t idp)
    {
        typedef cor::Pod::ChangeActiveResource_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), tid, idp).get(); 
    }

    idp_t GetCurrentActiveResource(size_t tid)
    {
        typedef cor::Pod::GetCurrentActiveResource_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), tid).get(); 
    }

    void InsertAgentMailbox(idp_t idp, hpx::id_type gid)
    {
        typedef cor::Pod::InsertAgentMailbox_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), idp, gid).get(); 
    }

    hpx::id_type GetAgentMailbox(idp_t idp)
    {
        typedef cor::Pod::GetAgentMailbox_action_pod action_type;
        return hpx::async<action_type>(this->get_id(), idp).get(); 
    }


    // void SendMessage(idp_t idp, idp_t dest, Message const& msg)
    // {
    //     typedef cor::Pod::SendMessage1_action_pod action_type;
    //     return hpx::async<action_type>(this->get_id(), idp, dest, msg).get(); 
    // }

    // void SendMessage(idp_t idp, std::vector<idp_t> const& dests, Message const& msg)
    // {
    //     typedef cor::Pod::SendMessage2_action_pod action_type;
    //     return hpx::async<action_type>(this->get_id(), idp, dests, msg).get(); 
    // }

    // Message ReceiveMessage(idp_t idp)
    // {
    //     typedef cor::Pod::ReceiveMessage1_action_pod action_type;
    //     return hpx::async<action_type>(this->get_id(), idp).get(); 
    // }

    // Message ReceiveMessage(idp_t idp, idp_t source)
    // {
    //     typedef cor::Pod::ReceiveMessage2_action_pod action_type;
    //     return hpx::async<action_type>(this->get_id(), idp, source).get(); 
    // }



};


}

#endif