#ifndef COR_CONTROLLER_HPP
#define COR_CONTROLLER_HPP

#include <hpx/hpx.hpp>
#include <string>
#include <thread>
#include <future>
#include <condition_variable>
#include <mutex>

#include "cor/system/macros.hpp"
#include "cor/services/page_manager.hpp"
#include "cor/services/resource_manager.hpp"

#include "cor/services/idp_manager_client.hpp"
#include "cor/services/resource_manager_global_client.hpp"
#include "cor/services/access_manager_client.hpp"
#include "cor/elements/mailbox_client.hpp"


namespace cor {

class ResourceManager;
class SessionManager;
class PageManager;
class Pod;
// class Mailbox_Client;

class Controller : std::enable_shared_from_this<Controller>
{

friend class PageManager;
friend class ResourceManager;

public:
    Controller();
    explicit Controller(std::string const& id, unsigned int pod_id, std::string const& app_group, std::string const& context, unsigned int npods);
    
    void StartService();
    void StopService();

    idp_t GenerateIdp();

    void InsertIdp(idp_t idp, hpx::id_type gid);
    bool FindIdp(idp_t idp);
    hpx::id_type GetGidFromIdp(idp_t idp);
    void RemoveIdp(idp_t idp);

    std::string GetGlobalContext();
    std::string GetLocalContext();

    unsigned int GetNumPods();
    unsigned int GetNumDomains();

    std::vector<idp_t> GetPods();
    std::vector<idp_t> GetDomains();

    // not global function, only local domain
    idp_t GetDomainIdp(idp_t idp);
    
    // local functions
    void InsertPredecessorIdp(idp_t idp, idp_t idp_predecessor);
    bool FindPredecessorIdp(idp_t idp);
    idp_t GetPredecessorIdp(idp_t idp);
    void RemovePredecessorIdp(idp_t idp);


    void InsertDynamicOrganizer_idps(idp_t idp);
    void InsertStaticOrganizer_idps(idp_t idp);
    bool FindDynamicOrganizer_idps(idp_t idp);
    bool FindStaticOrganizer_idps(idp_t idp);

    void InsertAgentMailbox(idp_t idp, hpx::id_type gid);
    hpx::id_type GetAgentMailbox(idp_t idp);

    template <typename T>
    std::unique_ptr<T> GetLocalResource(idp_t idp);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateLocal_test(idp_t ctx, std::string const& name, Args ... args);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, Args ... args);

    template <typename T, typename ... Args>
    idp_t Create(idp_t ctx, std::string const& name, Args ... args);

    template <typename T, typename ... Args>
    idp_t CreateRemote(idp_t ctx, std::string const& name, std::string const& ctrl, Args ... args);

    template <typename T>
    std::unique_ptr<T> CreateReference(idp_t idp, idp_t ctx, std::string const& name);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args ... args);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective(idm_t rank, idp_t comm, idp_t ctx, std::string const& name, Args ... args);

    idp_t Spawn(std::string const& context, unsigned int npods, unsigned int total_pods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts);

    //idp_t SpawnVirtualPod(std::string const& context, idp_t parent, std::string const& module, std::vector<std::string> const& args);

    // accessed by Container
    std::string SearchResource(idp_t idp);
    bool ContainsResource(idp_t idp);

    ~Controller() = default;
    Controller(Controller const&) = delete;
    Controller& operator=(Controller const&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(Controller&&) = delete;

    
protected:
    void Initialize();
    void Finalize();

    // interface to PageManager
    // request new page
    page_t RequestPage();

    // interface to ResourceManager
    // global functions
    void InsertPredecessorIdpGlobal(idp_t idp, idp_t idp_predecessor);
    bool FindPredecessorIdpGlobal(idp_t idp);
    idp_t GetPredecessorIdpGlobal(idp_t idp);
    void RemovePredecessorIdpGlobal(idp_t idp);

    void InsertIdpGlobal(idp_t idp, hpx::id_type gid);
    bool FindIdpGlobal(idp_t idp);
    hpx::id_type GetGidFromIdpGlobal(idp_t idp);
    void RemoveIdpGlobal(idp_t idp);

    void InsertDynamicOrganizer_idpsGlobal(idp_t idp);
    void InsertStaticOrganizer_idpsGlobal(idp_t idp);
    bool FindDynamicOrganizer_idpsGlobal(idp_t idp);
    bool FindStaticOrganizer_idpsGlobal(idp_t idp);

    void InsertAgentMailboxGlobal(idp_t idp, hpx::id_type gid);
    hpx::id_type GetAgentMailboxGlobal(idp_t idp);

private:
    std::string GetName() const;

    std::string _app_group;
    std::string _context;
    unsigned int _npods;

    unsigned int _init_total_npods;
    unsigned int _final_total_npods;
    unsigned int _init_ctx_npods;
    unsigned int _final_ctx_npods;

    std::condition_variable _cv;
    std::mutex _mtx;
    hpx::lcos::local::spinlock _mtx2;

    std::string hpx_address;
    int hpx_port;

    // std::string agas_address = "localhost"; //"192.168.85.245";
    // int agas_port = 1337;
    std::string agas_address;
    int agas_port;

    // services
    PageManager *_pg_mgr;
    ResourceManager *_rsc_mgr;
    SessionManager *_sess_mgr;

    // Objeto cliente do componente IdpManager, forcencedor global de idps
    IdpManager_Client *idpManager_object;

    // Objeto cliente do componente ResourceManagerGlobal, componente global que guarda informação sobre idps e gids dos recursos
    ResourceManagerGlobal_Client *resourceManagerGlobal_object;

    // Componente para decidir quem é o primeiro processo. Útil para saber quem cria a clausura
    AccessManager_Client *accessManager_object;

    unsigned int _pod_id;
};

}

#include "cor/services/controller.tpp"

#endif
