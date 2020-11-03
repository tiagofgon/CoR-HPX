


#include "cor/cor.hpp"

#include "cor/utils/utils.hpp"
// #include "cor/system/pod.hpp"
// #include "cor/system/rpc_manager.hpp"
// #include "cor/external/event/event.hpp"

// #include "cor/resources/domain_component.hpp"
#include "cor/system/pod_client.hpp"
// #include "cor/resource/domain_component_client.hpp"

namespace cor {

// hpx::id_type Initialize_hpx(std::string const& app_group, std::string const& context, unsigned int npods, std::string const& module)
std::unique_ptr<Domain_Client> Initialize_hpx(std::string const& app_group, std::string const& context, unsigned int npods, std::string const& module)
{
	// std::cout << "OLA_COR.CPP_id:" << hpx::get_locality_id() << std::endl;
    // enable libevent multithreaded environment
    // ev::thread::evthread_use_pthreads();
	
    // generate a random id for cor services
    auto id = utils::random_string(9);

    // create and start rpc manager
    // global::rpc = new RpcManager(id);
    // global::rpc->StartService();

    // // create and initialize pod
    // auto id2 = utils::random_string(9);
    // global::pod = new Pod(id2, app_group, context, npods);
    // global::pod->Initialize();


    // // create and initialize pod_component
    // auto newpod = hpx::local_new<cor::Pod_Component>(id, app_group, context, npods);
    // global::podid = newpod.get();

    // // create local domain_component
    // typedef cor::Pod_Component::CreateLocal_action_pod<Domain> action_type;
	// hpx::async<action_type>(global::podid, cor::MetaDomain, "", module).get();

    // Create pod component and their client object for interface
    // hpx::shared_future<hpx::id_type> newpod = hpx::local_new<cor::Pod_Component>(id, app_group, context, npods);
    // cor::Pod_Component_Client pod_object2(std::move(newpod));
    // global::podid = newpod.get();
    // global::pod_object = new Pod_Component_Client(std::move(newpod));
    global::pod = new Pod_Client(id, app_group, context, npods);
    global::pod->Initialize();

    // create local domain and return their Client object
    return global::pod->CreateLocal<Domain_Client>(cor::MetaDomain, "", module);
    
    // Domain_Client domain(std::move(domain_gid));
    // return domain;
    // return global::podid;
}

void Finalize_hpx()
{
    // global::pod->Finalize();
}

std::shared_ptr<Domain_Client> GetDomain()
{
    // idp_t domain_idp = global::pod->GetDomainIdp();
    
    idp_t domain_idp = global::pod->GetDomainIdp();
    // std::cout << "domain_idp: " << domain_idp << std::endl;
    auto dom = global::pod->GetLocalResource<Domain_Client>(domain_idp);
    std::shared_ptr<Domain_Client> dom_shared = std::move(dom);
    return dom_shared;
}

}
