#ifdef COR_POD_HPP
// #include "cor/resources/teste.hpp"
namespace cor {


template <typename T>
std::function<T> Pod::LoadFunction(std::string const& module, std::string const& function)
{
    std::unique_lock<std::mutex> lk(_mtx);
    auto dylib = _modules.at(module);
    return dylib->LoadFunction<T>(function);

    std::function<T> f_display_42 = [](idp_t param) { std::cout << 42 << std::endl; return 4; };
    return f_display_42;
}

template <typename T>
std::unique_ptr<T> Pod::GetLocalResource(idp_t idp)
{
    return _ctrl->GetLocalResource<T>(idp);
}

template <typename T>
std::unique_ptr<T> Pod::CreateLocal_agent(idp_t ctx, std::string const& name, hpx::function<void(int)> const& func)
{
    return _ctrl->CreateLocal_agent<T>(ctx, name, func);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Pod::CreateLocal(idp_t ctx, std::string const& name, Args&& ... args)
{
    return _ctrl->CreateLocal<T, Args...>(ctx, name, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
idp_t Pod::Create(idp_t ctx, std::string const& name, Args&& ... args)
{
    // if(hpx::get_locality_id()==0) {
    //     std::vector<hpx::id_type> localities = hpx::find_remote_localities();
        
    //     std::cout << "Minha localidade: " << hpx::find_here() << std::endl;

    //     auto id_type = hpx::new_<Teste>(hpx::find_here()).get();

    //     for (hpx::id_type const& id : localities)
    //     {
    //         std::cout << "Localidade remota: " << id << std::endl;
    //         // std::cout << "O Dado está na localidade: " << data->GetLocalityGID() << std::endl;
    //         auto newgid = hpx::components::migrate<Teste>(id_type, id).get();

    //         std::cout << "O Dado está na localidade: " << hpx::get_colocation_id(hpx::launch::sync, newgid) << std::endl;

    //     }
    // }

    // std::cout << "idp_t: " << ctx << std::endl;
    // std::cout << "CreateLocal no Pod para adicionar ao ascendente recurso "<< ctx << std::endl;
    return _ctrl->Create<T, Args...>(ctx, name, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
idp_t Pod::CreateRemote(idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args)
{
    // std::cout << "CreateRemote no Pod para adicionar ao ascendente recurso "<< ctx << std::endl;
    return _ctrl->CreateRemote<T, Args...>(ctx, name, ctrl, std::forward<Args>(args)...);
}

template <typename T>
std::unique_ptr<T> Pod::CreateReference(idp_t idp, idp_t ctx, std::string const& name)
{
    return _ctrl->CreateReference<T>(idp, ctx, name);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Pod::CreateCollective1(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args)
{
    return _ctrl->CreateCollective<T, Args...>(ctx, name, total_members, std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
std::unique_ptr<T> Pod::CreateCollective2(idm_t rank, idp_t comm, idp_t ctx, std::string const& name, Args&& ... args)
{

    // if(hpx::get_locality_id()==0) {
    //     std::vector<hpx::id_type> localities = hpx::find_remote_localities();
        
    //     std::cout << "Minha localidade: " << hpx::find_here() << std::endl;

    //     auto id_type = hpx::new_<Teste>(hpx::find_here()).get();

    //     for (hpx::id_type const& id : localities)
    //     {
    //         std::cout << "Localidade remota: " << id << std::endl;
    //         // std::cout << "O Dado está na localidade: " << data->GetLocalityGID() << std::endl;
    //         auto newgid = hpx::components::migrate<Teste>(id_type, id).get();

    //         std::cout << "O Dado está na localidade: " << hpx::get_colocation_id(hpx::launch::sync, newgid) << std::endl;

    //     }
    // }


    return _ctrl->CreateCollective<T, Args...>(rank, comm, ctx, name, std::forward<Args>(args)...); 
}



}

#endif
