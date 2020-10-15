
#include <hpx/hpx_main.hpp>

// #include <hpx/hpx.hpp>
// #include <hpx/include/run_as.hpp>
// #include <hpx/hpx_start.hpp>

#include <hpx/include/iostreams.hpp>
#include <iostream>
#include <cstdlib>

// #include "corhpx.cpp"
// #include "cor/cor.hpp"
#include "cor/cor_hpx.hpp"
// #include "cor/resources/domain_component_client.hpp"
// #include "cor/resources/group_component_client.hpp"
// #include "cor/resources/agent_component.hpp"
// #include "cor/resources/data_component.hpp"
// #include "cor/elements/static_organizer_component.hpp"
// #include "cor/elements/dynamic_organizer_component.hpp"
// #include "cor/elements/value_component.hpp"

// #include "cor/resources/data_component_client.hpp"
// #include "cor/resources/agent_component_client.hpp"
// #include "cor/resources/domain_component.hpp"



typedef void (*vfc)(int str);
// auto asd = decltype(vfc);

double foo(int, long);
typedef std::function< decltype(foo) > FooType;

typedef double mytype;


// REGISTER_PROTOAGENT(void, int);
// REGISTER_AGENT_COMPONENT(idp_t, idp_t);
// REGISTER_DATA_COMPONENT(mytype);
// REGISTER_DATA_COMPONENT(idp_t);
// REGISTER_DATA_COMPONENT(int);

// REGISTER_VALUE_COMPONENT(int);
    // REGISTER_VALUE_COMPONENT(int)


int funcaoTeste (int n1, int n2) {
    return n1+n2;
}

// void minhafuncao1(std::string const& app_group, std::string const& context, unsigned int npods, std::string const& module) {
//     auto domain_gid = cor::Initialize_hpx(app_group, context, npods, module);
//     cor::Domain_Component_Client domain(std::move(domain_gid));
//     auto agent = domain.CreateLocal<cor::Domain_Component>(cor::MetaDomain, "", module);
// }

int main(int argc, char *argv[])
{
    std::string app_group, context, module;
    unsigned int npods;
    unsigned int total_members;
    idp_t parent;
 	// std::cout << "OLA_id:" << hpx::get_locality_id() << std::endl;  
    if (argc >= 6) {
        app_group.assign(argv[1]);
        context.assign(argv[2]);
        total_members = npods = std::strtoul(argv[3], nullptr, 0);
        parent = std::strtoul(argv[4], nullptr, 0);
        module.assign(argv[5]);
        for (int i = 0; i < 6; ++i, --argc, ++argv);
    } 
    // std::cout << "threadthreadthreadthreadthread1: " << hpx::threads::get_self_id() << std::endl;
    // if (argc >= 7) {
    //     app_group.assign(argv[1]);
    //     context.assign(argv[2]);
    //     total_members = npods = std::strtoul(argv[3], nullptr, 0);
    //     parent = std::strtoul(argv[4], nullptr, 0);
    //     module.assign(argv[5]);
    //     for (int i = 0; i < 7; ++i, --argc, ++argv);
    // } else {
    //     std::cerr << "Error: Wrong number of arguments\n";
    //     std::cout << "Usage: " << argv[0] << " <app group> <context> <number pods> <parent> <module> <args...>\n";
    //     return EXIT_FAILURE;
    // }

    // This global object will initialize HPX in its constructor and make sure HPX
    // stops running in its destructor.
    // manage_global_runtime init(atoi(argv[6]));
    // manage_global_runtime init(0);
    // std::cout << atoi(argv[6]) << std::endl;
    // std::cout << "corhpx: " << hpx::get_locality_id() << std::endl;




    auto domain = cor::Initialize_hpx(app_group, context, npods, module);


    // minhafuncao1(app_group, context, npods, module);
    // cor::Domain_Component_Client domain(std::move(domain_gid));
    // // // auto agent = domain.CreateLocal<cor::Agent_Component<idp_t(idp_t)>>(5, "", domain.GetModuleName(), "Main");
    std::string nome("group");
    std::string main("Main");
    std::string const& basePathExternal = module ;
    std::string const& group = nome ;
    std::string const& Main = main ;



    // // auto agent = domain.CreateLocal<cor::Group_Component>(cor::MetaDomain, group, basePathExternal);
    // auto agent_gid = domain.CreateLocal<cor::Agent_Component<idp_t(idp_t)>>(cor::MetaDomain,  "", domain.GetModuleName(), Main);
    // cor::Agent_Component_Client<idp_t(idp_t)> agent(std::move(agent_gid));
    // agent.Run(559);

    // agent.Wait();
    // auto resultado = agent.Get();
    // std::cout << "resultadoooo: " << resultado  << std::endl;

{
    idp_t const& npodss = npods ;
    unsigned int const& total_memberss = total_members ;
    idp_t const& parentt = parent ;
    auto clos = domain->CreateCollective<cor::Closure_Client>(domain->Idp(), "", npods, total_memberss, parentt);
    
    
    
    
    //Closure_Component_Client(idp_t idp, unsigned int total_members, idp_t parent)
    // auto clos = domain->CreateLocal3<cor::Closure_Component_Client>(domain->Idp(), "", total_memberss, parentt);

    // auto agent = domain->CreateLocal3<cor::Agent_Component_Client<idp_t(idp_t)>>(cor::MetaDomain,  "", domain->GetModuleName(), Main);
    std::cout << "************ 1ª etapa *******"  << std::endl;
    auto agent = domain->CreateLocal<cor::ProtoAgent_Client<void(int)>>(clos->Idp(),  "", domain->GetModuleName(), Main);

    // hpx::lcos::barrier barrier_so("cacaca", 2);
    // // std::cout << "total_members: " << _total_members << ", o idp do meu static_organizer é: " << _idp << ", i: " << ++i << std::endl;
    // barrier_so.wait();

    std::cout << "************ 2ª etapa *******"  << std::endl;
    // cor::Agent_Component_Client<idp_t(idp_t)> agent(std::move(agent_gid));
    

    // std::string const& name = "group";
    // std::string const& path = "/opt/placor-hpx/examples/libcallableModule.so"; // é necessario fazer estas atribuições porque os argumentos de componentes tem de ser const
    // auto group2 = domain->CreateLocal3<cor::Group_Component_Client>(domain->Idp(), name, path);
    

    // std::string const& name2 = "data";
    // auto data = domain->CreateLocal3<cor::Data_Component_Client<idp_t>>(cor::MetaDomain, name2);
    
    // std::cout << "domain->Idp(): " << domain->Idp()  << std::endl;
    // std::cout << "agent->Idp(): " << agent->Idp()  << std::endl;
    // std::cout << "group->Idp(): " << group2->Idp()  << std::endl;
    // std::cout << "data->Idp(): " << data->Idp()  << std::endl;

    agent->Run(559);
    agent->Wait();
    agent->Get();
}

    // auto resultado = agent->Get();
    // std::cout << "resultadoooo: " << resultado  << std::endl;
    // std::cout << "threadthreadthreadthreadthread1: " << hpx::threads::get_self_id() << std::endl;

    // typedef cor::Domain_Component::GetModuleName_action_dynamicOrganizer action_type;
    // auto modulename = hpx::async<action_type>(domain_gid).get();

    // typedef cor::Domain_Component::CreateLocal_action_container<cor::Agent<void(int,char**)>> action_type2;
    // auto agent = hpx::async<action_type2>(domain_gid, 7, "", modulename, "Main").get();

    {
        // hpx::future<hpx::id_type> f = hpx::local_new<cor::Agent_Component<int(int, int)>>(66, funcaoTeste);
        // cor::Agent_Component_Client<int(int, int)> c(std::move(f));
        // // cor::Agent_Component_Client<int(int)> c = hpx::new_<cor::Agent_Component<int(int)>>(here, 2, funcaoTeste);

        // c.Run(554, 6);
        // c.Wait();
        // auto res = c.Get();
        // std::cout << "res funcaoteste = : " << res << std::endl;

        // // typedef typename cor::Agent_Component<int(int, int)>::Idp_action action_type;
        // // auto nn = hpx::async<action_type>(f.get()).get();
        // // std::cout << "O idp do Executor_componente é: " << nn << std::endl;

    }

    // {
    //     auto clos = domain->CreateCollective<cor::Closure>(domain.Idp(), "", npods, total_members, parent);

    //     auto agent = domain->CreateLocal<cor::Agent<void(int,char**)>>(clos.Idp(), "", domain->GetModuleName(), "Main");
    //     // agent->Run(argc, argv);
    //     // agent->Wait();
    //     // agent->Get();
    //     agent->Run_hpx(argc, argv);
    //     agent->Get_hpx();
    //     // auto agent = domain->CreateLocal<cor::Agent<idp_t(idp_t)>>(clos.Idp(), "", domain->GetModuleName(), "Main");
    //     // int jj=9999;
    //     // agent->Run_hpx(jj);
    // }

    // auto clos = domain->CreateCollective<cor::Closure>(domain.Idp(), "", npods, total_members, parent);

    // hpx::id_type here = hpx::find_here();
    // cor::Domain_Component_Client obj(hpx::components::new_<cor::Domain_Component>(hpx::find_here(), 2) );
    // idp_t aua = obj.Idp().get();
    // std::cout << "resultado da action:" << aua << std::endl;

    // cor::Group_Component_Client obj(hpx::components::new_<cor::Group_Component>(hpx::find_here(), 2) );
    // idp_t aua = obj.Idp().get();
    // std::cout << "resultado da action:" << aua << std::endl;
 


    // hpx::future<hpx::id_type> f = hpx::local_new<cor::Agent_Component<void(int)>>(2); 
    // typedef typename cor::Agent_Component<void(int)>::OriginalIdp_action action_type;
	// auto nn = hpx::async<action_type>(f.get()).get();
    // std::cout << "O idp do Executor_componente é: " << nn << std::endl;



    
    // hpx::future<hpx::id_type> f = hpx::local_new<cor::Domain_Component>(2);
    // typedef typename cor::Domain_Component::GetDynamicIdp_action action_type;
	// auto nn = hpx::async<action_type>(f.get()).get();
    // std::cout << "O idp do Domain_Component é: " << nn << std::endl;


    // hpx::local_new<cor::Data_Component<int>>(2, 98);


    
    /*   teste com Data_Component    */
    // hpx::future<hpx::id_type> f = hpx::local_new<cor::Data_Component<mytype>>(7, 92.08);
    // typedef typename cor::Data_Component<mytype>::fetch_action action_type;
    // auto c = f.get();
	// auto nn = hpx::async<action_type>(c).get();
    // std::cout << "O idp do Value_componente é: " << nn << std::endl;
    
    // std::shared_ptr<cor::Data_Component<mytype>> ptr = hpx::get_ptr<cor::Data_Component<mytype>>(hpx::launch::sync, c);
    // auto bv = **ptr;
    // std::cout << "**ptr1: " << bv << std::endl;
    
    // typedef typename cor::Data_Component<mytype>::GetValue_action action_type2;
    // auto nnn = hpx::async<action_type2>(c).get();
    // std::cout << "O novo apontador para value_ é: " << *nnn << std::endl;



    /*   teste com Agente_Component     */
    //  hpx::future<hpx::id_type> f = hpx::local_new<cor::Agent_Component<int(int)>>(2, funcaoTeste);
    //  auto c = f.get();

    // {
    //     auto c = f.get(); 
    //     typedef typename cor::Agent_Component<void(int)>::OriginalIdp_action action_type;
    //     auto nn = hpx::async<action_type>(c).get();
    //     std::cout << "O idp do Executor_componente é: " << nn << std::endl;
    // }

    // {
    //     typedef typename cor::Agent_Component<void(int)>::add_action<int> action_type;
    //     hpx::async<action_type>(c, 9).get();
    //     std::cout << "Executada a função: " << std::endl;

    // }

    // {
    //     typedef typename cor::Agent_Component<int(int)>::Run_action<int> action_type;
    //     hpx::async<action_type>(c, 666).get();
    //     // std::cout << "Executada a função: " << std::endl;

    //     typedef typename cor::Agent_Component<int(int)>::Wait_action action_type2;
    //     hpx::async<action_type2>(c).get();

    //     typedef typename cor::Agent_Component<int(int)>::Get_action action_type3;
    //     auto res = hpx::async<action_type3>(c).get();
    //     std::cout << "res = : " << res << std::endl;
    // }






    {
        // hpx::id_type here = hpx::find_here();
        // hpx::future<hpx::id_type> f = hpx::local_new<cor::Agent_Component<int(int, int)>>(66, funcaoTeste);
        // cor::Agent_Component_Client<int(int, int)> c(std::move(f));
        // // cor::Agent_Component_Client<int(int)> c = hpx::new_<cor::Agent_Component<int(int)>>(here, 2, funcaoTeste);

        // c.Run(554, 6);
        // c.Wait();
        // auto res = c.Get();
        // std::cout << "res funcaoteste = : " << res << std::endl;

        // // typedef typename cor::Agent_Component<int(int, int)>::Idp_action action_type;
        // // auto nn = hpx::async<action_type>(f.get()).get();
        // // std::cout << "O idp do Executor_componente é: " << nn << std::endl;

    }







    // // mytype t = "TIAGO";
    // hpx::future<hpx::id_type> f = hpx::new_<cor::Data_Component<mytype>>(here, 3, 778);
    // typedef typename cor::Data_Component<mytype>::Idp_action action_type;
    // auto c = f.get();
	// auto nn = hpx::async<action_type>(c);
    // std::cout << "O idp do Executor_componente é: " << nn.get() << std::endl;

    // typedef typename cor::Value_Component<mytype>::fetch_action action_type;
    // auto c = f.get();
	// auto nn = hpx::async<action_type>(c);
    // std::cout << "O objeto do Data_Component é: " << nn.get() << std::endl;
    // std::shared_ptr<cor::Data_Component<mytype>> ptr = hpx::get_ptr<cor::Data_Component<mytype>>(hpx::launch::sync, c);
    // auto bv = **ptr;
    // std::cout << "**ptr1: " << bv << std::endl;
    // **ptr = 111;
    // bv = **ptr;
    // std::cout << "**ptr2: " << bv << std::endl;

    // cor::Data_Component_Client<int> data_object(2, "name", 99);

    // *data_object = 50;
    // int a = data_object.getIdpValue().get();
    // std::cout << "O idp do Value_componente é: " << *data_object << std::endl;





    // {
    //     auto clos = domain->CreateCollective<cor::Closure>(domain.Idp(), "", npods, total_members, parent);

    //     auto agent = domain->CreateLocal<cor::Agent<void(int,char**)>>(clos.Idp(), "", domain->GetModuleName(), "Main");
    //     // agent->Run(argc, argv);
    //     // agent->Wait();
    //     // agent->Get();
    //     agent->Run_hpx(argc, argv);
    //     agent->Get_hpx();
    //     // auto agent = domain->CreateLocal<cor::Agent<idp_t(idp_t)>>(clos.Idp(), "", domain->GetModuleName(), "Main");
    //     // int jj=9999;
    //     // agent->Run_hpx(jj);
    // }

    // cor::Finalize_hpx();

    // void *handle;
    
    // char *error;

    // handle = dlopen("/opt/placor-hpx/examples/libmodule_hpx.so", RTLD_LAZY);
    // if (!handle) {
    //     fprintf(stderr, "%s\n", dlerror());
    //     exit(1);
    // }


    // typedef void (*Main_t)(int, char **);

    // dlerror();
    // Main_t Main = (Main_t) dlsym(handle, "Main");
    // const char *dlsym_error = dlerror();
    // if (dlsym_error) {
    //     std::cerr << "Cannot load symbol 'Main': " << dlsym_error << '\n';
    // } 
    
    // char **aux;
    // Main(0, aux);




    std::cout << "fim" << std::endl;


    return EXIT_SUCCESS;
}
