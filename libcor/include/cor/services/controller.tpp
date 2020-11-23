#ifdef COR_CONTROLLER_HPP

// #include "cor/system/pod_component_client.hpp"
// #include "cor/services/access_manager_client.hpp"
#include "cor/resources/resource.hpp"

namespace cor {

template <typename T>
std::unique_ptr<T> Controller::GetLocalResource(idp_t idp)
{
    return _rsc_mgr->GetLocalResource<T>(idp);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Controller::CreateLocal(idp_t ctx, std::string const& name, Args&& ... args)
{
    // std::cout << "idp_t_createLocal_Controller: "<< ctx << std::endl;
    return _rsc_mgr->CreateLocal<T, Args...>(ctx, name, GetName(), std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
idp_t Controller::CreateRemote(idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args)
{
    //std::cout << "Controller::CreateRemote" << std::endl;
    // std::cout << "idp_t_createRemote_Controller: "<< ctx << std::endl;
    return _rsc_mgr->CreateRemote<T, Args...>(ctx, name, ctrl, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
idp_t Controller::Create(idp_t ctx, std::string const& name, Args&& ... args)
{
    return _rsc_mgr->Create<T>(ctx, name, GetName(), std::forward<Args>(args)...);
}

template <typename T>
std::unique_ptr<T> Controller::CreateReference(idp_t idp, idp_t ctx, std::string const& name)
{
    return _rsc_mgr->CreateReference<T>(idp, ctx, name, GetName());
}

template <typename T, typename ... Args>
std::unique_ptr<T> Controller::CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args)
{
    std::unique_ptr<T> rsc_ptr;
    std::string barrier_name = _context + "barrier";
    //std::cout << "\n_context: " << _context << ", barrier_name:" << barrier_name << ", with: " << total_members << " members\n" << std::endl;

    auto first = hpx::get_locality_id();
    std::string basename = name + "basename" + _context;
    int pos = accessManager_object->GetPosition(_context);
    
    //std::cout << "aqui3, na localidade: " << first << std::endl;
    if(pos==0) { // Primeira localidade, cria o recurso e regista-o no agas
        //std::cout << "registou" << std::endl;
        rsc_ptr = CreateLocal<T>(ctx, name, std::forward<Args>(args)...);
        hpx::register_with_basename(basename, rsc_ptr->GetGid(), 0);

        auto parent = rsc_ptr->GetParent();
        auto clos_idp = rsc_ptr->Idp();
        // enviar ao agente que fez o spawn, o idp da clausura
        if(parent != 0) {
            cor::Message msg;
            msg.Add<std::string>(_context);
            msg.SetSender(clos_idp);
            
            std::vector<char> buffer;
            hpx::serialization::output_archive oarchive(buffer);
            oarchive << msg;

            typedef std::vector<char> data_type;
            std::string channel_name = "Mailbox_ctx-" + std::to_string(parent);
            hpx::lcos::channel<data_type> send_channel;
            send_channel.connect_to(channel_name);
            send_channel.set(buffer);
        }

        if(total_members > 1) 
        {
            //std::cout << "dentro da barreira-0" << std::endl;
            hpx::lcos::barrier barrier(barrier_name, total_members, pos); // rank == 0, a barreira tem de ter obrigatoriamente o rank 0
            //std::cout << "fora da barreira-0" << std::endl;
            barrier.wait();
        }

        // Enviar uma mensagem ao spawn com o idp da clausura


    }

    if (pos!=0) { // O resto das localidades
        //std::cout << "find1" << std::endl;
        // Procurar o GID do recurso com nome "name"
		auto gid = hpx::find_from_basename(basename, 0).get();
        // std::cout << "aqui1" << std::endl;
        typedef Resource::Idp_action_Resource action_type;
        auto idp = hpx::async<action_type>(gid).get();
        // std::cout << "aqui2" << std::endl;
        // Cria uma referencia para o componente identificado por idp que pertence ao contexto ctx
        rsc_ptr = CreateReference<T>(idp, ctx, name);
        if(total_members > 1) 
        {
            //std::cout << "dentro da barreira" << std::endl;
            hpx::lcos::barrier barrier(barrier_name, total_members, pos);
            //std::cout << "fora da barreira" << std::endl;
            barrier.wait();
        }

    }





    // if(first==0) { // Primeira localidade, cria o recurso e regista-o no agas
    //     rsc_ptr = CreateLocal<T>(ctx, name, std::forward<Args>(args)...);
    //     hpx::register_with_basename(basename, rsc_ptr->GetGid());
    // }

    // if (first!=0) { // O resto das localidades

    //     // Procurar o GID do recurso com nome "name"
	// 	auto gid = hpx::find_from_basename(basename, 0).get();
    //     // std::cout << "aqui1" << std::endl;
    //     typedef Resource::Idp_action_Resource action_type;
    //     auto idp = hpx::async<action_type>(gid).get();
    //     // std::cout << "aqui2" << std::endl;
    //     // Cria uma referencia para o componente identificado por idp que pertence ao contexto ctx
    //     rsc_ptr = CreateReference<T>(idp, ctx, name);
    // }

    // std::string nomebarr = "barreira" + std::to_string(hpx::get_locality_id());
    // std::string const& nomebar = nomebarr;
    // std::cout << nomebar << std::endl;
    // //hpx::lcos::local::barrier barrier(1);
    // //hpx::lcos::barrier barrier(nomebar, 1);
    // //hpx::lcos::barrier barrier(barrier_name, 1);
    // std::cout << "\nbarrier with name -" << barrier_name << "- for -" << total_members << "- members\n" << std::endl;
    // // Esperar que todas as threads distribuidas cheguem aqui, ou seja, total_members
    // //barrier.wait();
    // // std::cout << "barrier2" << std::endl;

    return rsc_ptr;
}

template <typename T, typename ... Args>
std::unique_ptr<T> Controller::CreateCollective(idm_t rank, idp_t comm, idp_t ctx, std::string const& name, Args&& ... args)
{
    return _rsc_mgr->CreateCollective<T>(rank, comm, ctx, name, GetName(), std::forward<Args>(args)...);
}



}

#endif
