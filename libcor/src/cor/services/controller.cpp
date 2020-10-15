//#include "cor/cor.hpp"



#include <hpx/hpx.hpp>

#include "cor/services/controller.hpp"

#include "cor/utils/utils.hpp"
#include "cor/services/page_manager.hpp"
#include "cor/services/session_manager.hpp"

// #include <thread>
// #include <sstream>

// #include <iostream> // to remove


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
    _rsc_mgr{nullptr}
    // _sess_mgr{nullptr}
{
    idpManager_object = new IdpManager_Client();
    resourceManagerGlobal_object = new ResourceManagerGlobal_Client();
}

void Controller::StartService()
{
    // initialize global context
    Initialize();

    // // create page manager initial context
    _pg_mgr->CreateInitialContext();

    // // create resource manager initial context
    _rsc_mgr->CreateInitialContext(GetName());
}


void Controller::StopService()
{
    Finalize();
}


void Controller::Initialize()
{
    std::cout << "Controller::Initialize()" << std::endl;
    // join app group and synchronize
    // _mbox->join(_app_group);
    bool _is_main_ctrl=false;
    if(hpx::get_locality_id()==0)
        _is_main_ctrl = true;

    // // create page and resource managers
    _pg_mgr = new PageManager(this);
    _rsc_mgr = new ResourceManager(this, _is_main_ctrl);
    // _sess_mgr = new SessionManager();
}

void Controller::Finalize()
{

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
    return resourceManagerGlobal_object->FindIdp(idp);
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
    std::unique_lock<std::mutex> lk(_mtx);
    return _init_total_npods;
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

void Controller::InsertDynamicOrganizer_idpsGlobal(idp_t idp)
{
    std::cout << "cacacac5" << std::endl;
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




// idp_t Controller::Spawn(std::string const& context, unsigned int npods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
// {

// }

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
