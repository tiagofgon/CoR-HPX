
// #include <hpx/hpx_main.hpp>
#include <hpx/hpx_init.hpp>


// #include <hpx/hpx.hpp>
// #include <hpx/include/run_as.hpp>
// #include <hpx/hpx_start.hpp>

#include <hpx/include/iostreams.hpp>
#include <iostream>
#include <cstdlib>

// #include "corhpx.cpp"
// #include "cor/cor.hpp"
#include "cor/cor.hpp"
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


int hpx_main(int argc, char *argv[])
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


    auto domain = cor::Initialize_hpx(app_group, context, npods, module);


    // minhafuncao1(app_group, context, npods, module);
    // cor::Domain_Component_Client domain(std::move(domain_gid));
    // // // auto agent = domain.CreateLocal<cor::Agent_Component<idp_t(idp_t)>>(5, "", domain.GetModuleName(), "Main");
    std::string nome("group");
    std::string main("Main");
    std::string const& basePathExternal = module ;
    std::string const& group = nome ;
    std::string const& Main = main ;



    idp_t const& npodss = npods ;
    unsigned int const& total_memberss = total_members ;
    idp_t const& parentt = parent ;
    auto clos = domain->CreateCollective<cor::Closure_Client>(domain->Idp(), "", npods, total_memberss, parentt);
    

    std::cout << "************ Criação do agente principal no corhpx *******"  << std::endl;
    auto agent = domain->CreateLocal<cor::ProtoAgent_Client<void(int)>>(clos->Idp(),  "", domain->GetModuleName(), Main);

    std::cout << "************ Execução do modulo *******"  << std::endl;


    agent->Run(559);
    agent->Wait();
    agent->Get();


    std::cout << "fim" << std::endl;


    // return EXIT_SUCCESS;
    return hpx::finalize();
}


int main(int argc, char * argv[])
{
    return hpx::init(argc, argv);
}
