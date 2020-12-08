//#include "cor/cor.hpp"



//#include <hpx/hpx.hpp>

#include "cor/services/controller.hpp"

#include "cor/utils/utils.hpp"
#include "cor/services/page_manager.hpp"
#include "cor/services/session_manager.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include "cor/elements/mailbox_client.hpp"


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

// #include <thread>
// #include <sstream>

// #include <iostream> // to remove


/* Auxiliar functions */
// check if the given string is a numeric string or not
bool chkNumber(const std::string& str){
   return !str.empty() &&
   (str.find_first_not_of("[0123456789]") == std::string::npos);
}
// Function to split string str using given delimiter
std::vector<std::string> split(const std::string& str, char delim){
   auto i = 0;
   std::vector<std::string> list;
   auto pos = str.find(delim);
   while (pos != std::string::npos){
      list.push_back(str.substr(i, pos - i));
      i = ++pos;
      pos = str.find(delim, pos);
   }
   list.push_back(str.substr(i, str.length()));
   return list;
}
// Function to validate an IP address
bool validateIP(std::string ip){
   // split the string into tokens
   std::vector<std::string> slist = split(ip, '.');
   // if token size is not equal to four
   if (slist.size() != 4)
      return false;
   for (std::string str : slist){
      // check that string is number, positive, and range
      if (!chkNumber(str) || stoi(str) < 0 || stoi(str) > 255)
         return false;
   }
   return true;
}

std::string getIpAddress(std::string ip_name) {
    if(validateIP(ip_name)) {
        return ip_name;
    }
    if(ip_name == "localhost") {
        // To retrieve hostname
        char hostbuffer[256];
        int hostname;
        hostname = gethostname(hostbuffer, sizeof(hostbuffer));
        hostent * record = gethostbyname(hostbuffer);
        in_addr * address = (in_addr * )record->h_addr;
        std::string ip_address = inet_ntoa(* address);
        //std::cout << "ip_name : " << ip_name << " - " << ip_address << std::endl;
        return ip_address;
    }
    else {
        hostent * record = gethostbyname(ip_name.c_str());
        in_addr * address = (in_addr * )record->h_addr;
        std::string ip_address = inet_ntoa(* address);
        //std::cout << "ip_name : " << ip_name << " - " << ip_address << std::endl;
        return ip_address;
    }
}
/*  */




namespace cor {

Controller::Controller(std::string const& id, std::string const& app_group, std::string const& context, unsigned int npods) :
    _app_group{app_group},
    _context{context},
    _npods{npods},
    _init_total_npods{0},
    _final_total_npods{0},
    _init_ctx_npods{0},
    _final_ctx_npods{0},
    _cv{},
    _mtx{},
    _pg_mgr{nullptr},
    _rsc_mgr{nullptr},
    _sess_mgr{nullptr}
{
    idpManager_object = new IdpManager_Client();
    resourceManagerGlobal_object = new ResourceManagerGlobal_Client();
    accessManager_object = new AccessManager_Client(context);

    // Atualizar o ip e a porta do servidor agas
    agas_address = hpx::get_config_entry("hpx.agas.address", HPX_INITIAL_IP_ADDRESS);
    agas_port = std::stoi(hpx::get_config_entry("hpx.agas.port", std::to_string(HPX_INITIAL_IP_PORT)));

    // Atualizar o ip e a porta do hpx desta localidade
    hpx_address = hpx::get_config_entry("hpx.parcel.address", HPX_INITIAL_IP_ADDRESS);
    hpx_port = std::stoi(hpx::get_config_entry("hpx.parcel.port", std::to_string(HPX_INITIAL_IP_PORT)));
    // std::cout << "AddConnection " << hpx_address << ":" << hpx_port << std::endl; 
}

void Controller::StartService()
{
    // initialize global context
    Initialize();

    // // create page manager initial context
    _pg_mgr->CreateInitialContext();

    // // create resource manager initial context
    _rsc_mgr->CreateInitialContext(GetName());

    // adicionar esta localidade ao map de localidades deste contexto
    accessManager_object->AddMyContextLocality(_context, hpx::get_locality_id());

    // Adicionar esta conecção
    accessManager_object->AddConnection(hpx_address, hpx_port);
    
}


void Controller::StopService()
{
    Finalize();
}


void Controller::Initialize()
{
    //std::cout << "Controller::Initialize()" << std::endl;
    // join app group and synchronize
    // _mbox->join(_app_group);
    bool _is_main_ctrl=false;
    if(hpx::get_locality_id()==0)
        _is_main_ctrl = true;

    // // create page and resource managers
    _pg_mgr = new PageManager(this);
    _rsc_mgr = new ResourceManager(this, _is_main_ctrl);
    _sess_mgr = new SessionManager();
}

void Controller::Finalize()
{
    _sess_mgr->StopService();
    //accessManager_object->RemoveConnection(host, port);
}

idp_t Controller::GenerateIdp()
{
    return _pg_mgr->GenerateIdp();
}

void Controller::InsertIdp(idp_t idp, hpx::id_type gid)
{
    return _rsc_mgr->InsertIdp(idp, gid);
}

bool Controller::FindIdp(idp_t idp)
{
    return _rsc_mgr->FindIdp(idp);
}

hpx::id_type Controller::GetGidFromIdp(idp_t idp)
{
    return _rsc_mgr->GetGidFromIdp(idp);
}

void Controller::RemoveIdp(idp_t idp)
{
    return _rsc_mgr->RemoveIdp(idp);
}

void Controller::InsertIdpGlobal(idp_t idp, hpx::id_type gid)
{
    return resourceManagerGlobal_object->InsertIdp(idp, gid);
}

bool Controller::FindIdpGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->FindIdpGlobal(idp);
}

hpx::id_type Controller::GetGidFromIdpGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->GetGidFromIdp(idp);
}

void Controller::RemoveIdpGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->RemoveIdp(idp);
}

std::string Controller::GetGlobalContext()
{
    return _app_group;
}

std::string Controller::GetLocalContext()
{
    return _context;
}

unsigned int Controller::GetTotalPods()
{    
    _mtx2.lock();
    auto res = _init_total_npods;
    _mtx2.unlock();

    return res;
}

unsigned int Controller::GetTotalDomains()
{
    return _rsc_mgr->GetTotalDomains();
}

idp_t Controller::GetDomainIdp(idp_t idp)
{
    return _rsc_mgr->GetDomainIdp(idp);
}

void Controller::InsertPredecessorIdp(idp_t idp, idp_t idp_predecessor)
{
    return _rsc_mgr->InsertPredecessorIdp(idp, idp_predecessor);
}

idp_t Controller::GetPredecessorIdp(idp_t idp)
{
    return _rsc_mgr->GetPredecessorIdp(idp);
}

bool Controller::FindPredecessorIdp(idp_t idp)
{
    return _rsc_mgr->FindPredecessorIdp(idp);
}

void Controller::RemovePredecessorIdp(idp_t idp)
{
    return _rsc_mgr->RemovePredecessorIdp(idp);
}

void Controller::InsertPredecessorIdpGlobal(idp_t idp, idp_t idp_predecessor)
{
    return resourceManagerGlobal_object->InsertPredecessorIdp(idp, idp_predecessor);
}

idp_t Controller::GetPredecessorIdpGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->GetPredecessorIdp(idp);
}

bool Controller::FindPredecessorIdpGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->FindPredecessorIdp(idp);
}

void Controller::RemovePredecessorIdpGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->RemovePredecessorIdp(idp);
}



void Controller::InsertDynamicOrganizer_idps(idp_t idp)
{
    std::cout << "cacacac4" << std::endl;
    return _rsc_mgr->InsertDynamicOrganizer_idps(idp);
}

void Controller::InsertStaticOrganizer_idps(idp_t idp)
{
    return _rsc_mgr->InsertStaticOrganizer_idps(idp);
}

bool Controller::FindDynamicOrganizer_idps(idp_t idp)
{
    return _rsc_mgr->FindDynamicOrganizer_idps(idp);
}

bool Controller::FindStaticOrganizer_idps(idp_t idp)
{
    return _rsc_mgr->FindStaticOrganizer_idps(idp);
}

void Controller::InsertAgentMailbox(idp_t idp, hpx::id_type gid)
{
    return _rsc_mgr->InsertAgentMailbox(idp, gid);
}

hpx::id_type Controller::GetAgentMailbox(idp_t idp)
{
    return _rsc_mgr->GetAgentMailbox(idp);
}

void Controller::InsertDynamicOrganizer_idpsGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->InsertDynamicOrganizer_idps(idp);
}

void Controller::InsertStaticOrganizer_idpsGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->InsertStaticOrganizer_idps(idp);
}

bool Controller::FindDynamicOrganizer_idpsGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->FindDynamicOrganizer_idps(idp);
}

bool Controller::FindStaticOrganizer_idpsGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->FindStaticOrganizer_idps(idp);
}

void Controller::InsertAgentMailboxGlobal(idp_t idp, hpx::id_type gid)
{
    return resourceManagerGlobal_object->InsertAgentMailbox(idp, gid);
}

hpx::id_type Controller::GetAgentMailboxGlobal(idp_t idp)
{
    return resourceManagerGlobal_object->GetAgentMailbox(idp);
}




idp_t Controller::Spawn(std::string const& context, unsigned int npods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
{

    // Para os processos filho saberem quem é o primeiro, é adicionado um novo context ao accessManager_object
    accessManager_object->AddContext(context);

    // tranformar os endereços dns para ips
    auto hosts_ips = hosts;
    for (int i = 0; i < hosts.size(); ++i) {
        hosts_ips[i] = getIpAddress(hosts[i]);
    }

    // spawn pods
    for (int i = 0; i < npods; ++i) {
    auto pos = i % hosts.size();

    // assemble command
    std::string cmd;
    cmd.append("corhpx");
    cmd.append(" ");
    cmd.append(_app_group);
    cmd.append(" ");
    cmd.append(context);
    cmd.append(" ");
    cmd.append(std::to_string(npods));
    cmd.append(" ");
    cmd.append(std::to_string(parent));
    cmd.append(" ");
    cmd.append(module);
    for (int i = 0; i < args.size(); ++i) {
        cmd.append(" ");
        cmd.append(args[i]);
    }
    cmd.append(" ");

    

    // hpx command lines
    unsigned int port_int = accessManager_object->GetNextPort(hosts_ips[pos]); // buscar nova porta para o filho
    std::string host = hosts_ips[pos];
    std::string port = std::to_string(port_int);
        
    cmd.append("--hpx:hpx=" + host + ":" + port);
    // cmd.append("--hpx:hpx=" + host + ":1338");
    cmd.append(" ");
    cmd.append("--hpx:agas=" + agas_address + ":" + std::to_string(agas_port));
    // cmd.append("--hpx:agas=localhost:" + std::to_string(agas_port));
    cmd.append(" ");
    cmd.append("--hpx:run-hpx-main");
    cmd.append(" ");
    cmd.append("--hpx:expect-connecting-localities");
    cmd.append(" ");
    cmd.append("--hpx:worker");
    cmd.append(" ");
    cmd.append("--hpx:threads=2");
    cmd.append(" ");
    cmd.append("--hpx:ini=hpx.component_paths=/share/apps/placor-hpx/examples");

    //std::cout << "spawned comando: " << cmd << std::endl;


    _sess_mgr->CreateRemoteSession(host, "22", cmd);

    }

    hpx::id_type my_mailBox_gid = GetAgentMailbox(parent);
    Mailbox_Client my_mailbox(std::move(my_mailBox_gid));
    cor::Message msg;
    idp_t clos_idp = my_mailbox.ReceiveNewClos(context);
    return clos_idp;

}





std::string Controller::GetName() const
{
    return "naoseiquetediga";
}

std::string Controller::SearchResource(idp_t idp)
{
    return _rsc_mgr->SearchResource(idp);
}

bool Controller::ContainsResource(idp_t idp)
{
    return _rsc_mgr->ContainsResource(idp);
}

// Recebe o pedido do page_manager e envia a nova página proveniente do componente "idpManager_basename"
page_t Controller::RequestPage()
{
    page_t page = idpManager_object->GeneratePage();

    return page;
}








}
