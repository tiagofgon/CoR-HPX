
#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>

#include <iostream>
#include <cstdlib>

#include "cor/cor.hpp"
#include "cor/PlaCoRConfig.h"

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
 
    if (argc >= 6) {
        app_group.assign(argv[1]);
        context.assign(argv[2]);
        total_members = npods = std::strtoul(argv[3], nullptr, 0);
        parent = std::strtoul(argv[4], nullptr, 0);
        module.assign(argv[5]);
        for (int i = 0; i < 6; ++i, --argc, ++argv);
    } 

    auto domain = cor::Initialize_hpx(app_group, context, npods, module);

    auto clos = domain->CreateCollective<cor::Closure_Client>(domain->Idp(), "", npods, total_members, parent);


    //std::cout << "************ Criação do agente principal no corhpx *******"  << std::endl;
    auto agent = domain->CreateLocal<cor::Agent_Client<void(int,char**)>>(clos->Idp(),  "", domain->GetModuleName(), "Main");

    //std::cout << "************ Execução do modulo *******"  << std::endl;
    auto fut = agent->Run(argc, argv);
    fut.get();


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
    "hpx.run_hpx_main!=1", // run this code on all localities
    "hpx.expect_connecting_localities!=1" // Make sure networking will not be disabled
    };
    cfg.push_back(endereco); // run runtime on this ip address

    return hpx::init(argc, argv, cfg); // start runtime
}
