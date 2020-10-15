


#include "cor/cor.hpp"

#include "cor/utils/utils.hpp"
#include "cor/system/pod.hpp"
// #include "cor/system/rpc_manager.hpp"
#include "cor/external/event/event.hpp"

// #include "cor/system/pod_component_client.hpp"

namespace cor {

ResourcePtr<Domain> Initialize(std::string const& app_group, std::string const& context, unsigned int npods, std::string const& module)
{
	// std::cout << "OLA_COR.CPP_id:" << hpx::get_locality_id() << std::endl;
    // enable libevent multithreaded environment
    ev::thread::evthread_use_pthreads();
	
    // generate a random id for cor services
    auto id = utils::random_string(9);

    // create and start rpc manager
    // global::rpc = new RpcManager(id);
    // global::rpc->StartService();

    // create and initialize pod
    auto id2 = utils::random_string(9);
    global::pod = new Pod(id2, app_group, context, npods);
    global::pod->Initialize();


    // // create and initialize pod_component
    // auto newpod = hpx::local_new<cor::Pod_Component>(id, app_group, context, npods);
    // global::podid = newpod.get();

    // // create local domain_component
    // typedef cor::Pod_Component::CreateLocal_action_pod<Domain> action_type;
	// hpx::async<action_type>(global::podid, cor::MetaDomain, "", module).get();
    // hpx::future<hpx::id_type> newpod = hpx::local_new<cor::Pod_Component>(id, app_group, context, npods);
    // cor::Pod_Component_Client c(std::move(newpod));
    // c.Initialize();
    // c.CreateLocal<Domain_Component>(cor::MetaDomain, "", module);

    // create local domain
    return global::pod->CreateLocal<Domain>(cor::MetaDomain, "", module);
}

void Finalize()
{
    global::pod->Finalize();
}

ResourcePtr<Domain> GetDomain()
{
    return global::pod->GetLocalResource<Domain>(global::pod->GetDomainIdp());
}

}
