
// #include <hpx/hpx_main.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>

// #include <hpx/hpx.hpp>
// #include <hpx/include/run_as.hpp>
// #include <hpx/hpx_start.hpp>

// #include <hpx/include/iostreams.hpp>
#include <iostream>
#include <cstdlib>

// #include "corhpx.cpp"
// #include "cor/cor.hpp"
#include "cor/cor.hpp"
#include "cor/PlaCoRConfig.h"
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
#include "cor/services/access_manager_client.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <ifaddrs.h>

struct PrintNum {
    void operator()(std::shared_ptr<void> arg)
    {
        std::cout << "Ola Tiago" << std::endl;
    }
};

struct PrintNum2 {
    void operator()(int arg)
    {
        std::cout << "Ola Tiago" << std::endl;
    }
};

int funcaoTeste (int n1, int n2) {
    return n1+n2;
}
void funcaoTeste2 (std::shared_ptr<void> arg) {
    std::cout << "Ola Tiago" << std::endl;
}
void funcaoTeste3 (void *arg) {
    std::cout << "Ola Tiago" << std::endl;
}
void funcaoTeste4 () {
    std::cout << "Ola Tiago" << std::endl;
}


hpx::function<void(std::shared_ptr<void> arg)> funcaoTeste5 = PrintNum();
hpx::function<void(int)> funcaoTeste6 = PrintNum2();

int hpx_main(int argc, char *argv[])
{
        
    if (argc < 2) {
        // report version
        std::cout << argv[0] << " Version " << PlaCoR_VERSION_MAJOR << "."
                << PlaCoR_VERSION_MINOR << std::endl;
        return hpx::finalize();
    }

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

    std::string nome("group");
    std::string main("Main");
    std::string const& basePathExternal = module ;
    std::string const& group = nome ;
    std::string const& Main = main ;





    idp_t const& npodss = npods ;
    unsigned int const& total_memberss = total_members ;
    idp_t const& parentt = parent ;
    auto clos = domain->CreateCollective<cor::Closure_Client>(domain->Idp().get(), "", npods, total_memberss, parentt).get();
    
    // auto agent2 = domain->CreateLocal_agent<cor::ProtoAgent_Client<void(void*)>>(clos->Idp(),  "", funcaoTeste5);
    // agent2->Run();
    // agent2->Wait();
    // agent2->Get();



    idp_t asda = 221;
    //cor::Operon<void(std::shared_ptr<void>)> operon(asda, funcaoTeste2);
    
    int num = 2;
    //cor::Operon_Client operon(asda, 2);
    // auto operon = domain->CreateLocal<cor::Operon_Client>(clos->Idp(),  "", 1);
    // operon->Dispatch(PrintNum());

    //auto operon = domain->CreateLocal_agent<cor::Operon<void(int)>>(clos->Idp(),  "", funcaoTeste6);
    //cor::ProtoAgent_Client<void(void*)> agent3(asda, funcaoTeste3);
    //cor::ProtoAgent_Client<void(std::shared_ptr<const void>)> agent3(asda, funcaoTeste2);
    //cor::ProtoAgent_Client<void()> agent3(asda, funcaoTeste4);




    //std::cout << "************ Criação do agente principal no corhpx *******"  << std::endl;
    auto agent = domain->CreateLocal<cor::Agent_Client<void(int)>>(clos->Idp().get(),  "", domain->GetModuleName().get(), Main).get();
    //auto agent = domain->CreateLocal<cor::Agent_Client<void(arg)>>(clos->Idp(),  "", domain->GetModuleName(), Main);

    //std::cout << "************ Execução do modulo *******"  << std::endl;

    agent->Run(559);
    agent->Wait();
    agent->Get();



    return hpx::finalize();
}


int main(int argc, char * argv[])
{

    // To retrieve hostname
    char hostbuffer[256];
    int hostname;
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    //std::cout << "address " << hostbuffer << std::endl;
    
    hostent * record = gethostbyname(hostbuffer);
    in_addr * address = (in_addr * )record->h_addr;
	std::string ip_address = inet_ntoa(* address);
    //std::cout << "ip_address " << ip_address << std::endl;

    std::string endereco = "hpx.parcel.address=" + ip_address;

    std::vector<std::string> cfg = {
    "hpx.run_hpx_main!=1",
    // Make sure networking will not be disabled
    "hpx.expect_connecting_localities!=1"
    // "hpx.parcel.port=7910"
    // "hpx.parcel.address=ip_address"
    };
    cfg.push_back(endereco);

    return hpx::init(argc, argv, cfg);
}
