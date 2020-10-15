#ifndef COR_RESOURCE_MANAGER_HPP
#define COR_RESOURCE_MANAGER_HPP

#include <map>
#include <mutex>
#include <tuple>
#include <future>
#include <typeindex>
#include <condition_variable>

#include "cor/resources/resource.hpp"
#include "cor/system/macros.hpp"

#include <hpx/hpx.hpp>

#include "cor/resources/resource.hpp"
// #include "cor/system/system.hpp"
// #include "cor/utils/utils.hpp"

// #include "cor/system/pod.hpp"

// #include "cor/services/controller.hpp"

namespace cor {

class Controller;


class ResourceManager
{

// template <typename> friend class ResourcePtr;

public:
    explicit ResourceManager(Controller *ctrl, bool first);
    ~ResourceManager();

    void CreateInitialContext(std::string const& ctrl);
    void CleanInitialContext();

    void CreateMetaDomain(std::string const& ctrl);
    void FindMetaDomain(std::string const& ctrl);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args);

    template <typename T, typename ... Args>
    idp_t CreateRemote(idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args);

    template <typename T, typename ... Args>
    idp_t Create(idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args);

    template <typename T>
    std::unique_ptr<T> CreateReference(idp_t idp, idp_t ctx, std::string const& name, std::string const& ctrl);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective(idm_t rank, idp_t comm, idp_t ctx, std::string const& name, std::string const& ctrl, Args&& ... args);

    bool ContainsResource(idp_t idp);

    template <typename T>
    std::unique_ptr<T> GetLocalResource(idp_t idp);

    unsigned int GetTotalDomains();

    // not global function, only local domain
    idp_t GetDomainIdp(idp_t idp);

    void InsertPredecessorIdp(idp_t idp, idp_t idp_predecessor);
    bool FindPredecessorIdp(idp_t idp);
    idp_t GetPredecessorIdp(idp_t idp);
    void RemovePredecessorIdp(idp_t idp);

    // interface to Controller
    void EraseResource(idp_t idp);

    void InsertIdp(idp_t idp, hpx::id_type gid);
    bool FindIdp(idp_t idp);
    hpx::id_type GetGidFromIdp(idp_t idp);
    void RemoveIdp(idp_t idp);

    void InsertDynamicOrganizer_idps(idp_t idp);
    void InsertStaticOrganizer_idps(idp_t idp);
    bool FindDynamicOrganizer_idps(idp_t idp);
    bool FindStaticOrganizer_idps(idp_t idp);




    void SendResourceAllocationInfo(idp_t idp);

    void FindGlobalResource(idp_t idp);
    void HandleFindGlobalResource(idp_t idp, std::string ctrl);
    void SendGlobalResourceFound(idp_t idp, std::string const& ctrl);
    void GlobalResourceFound(idp_t idp);

    void FindPredecessor(idp_t idp);
    void HandleFindPredecessor(idp_t idp, std::string ctrl);
    void SendPredecessorFound(idp_t idp, idp_t pred, std::string const& ctrl);
    void PredecessorFound(idp_t idp, idp_t pred);

    void ReleaseReplica(idp_t idp, std::string requester);
    void CheckReplica(idp_t idp, unsigned int size, std::string requester);

    void CheckUpdate(idp_t idp, std::string requester);
    // void Update(idp_t idp, Resource *rsc);

    void Invalidate(idp_t idp);

    void CheckTokenUpdate(idp_t idp, std::string requester);
    // void AcquireTokenUpdate(idp_t idp, Resource *rsc, std::string replier);

    void TokenAck(idp_t idp);

    void CreateStaticGroup(idp_t comm, unsigned int total_members);
    void HandleCreateStaticGroup(idp_t comm);

    void SendStaticGroupCCIdp(idp_t comm, idp_t idp);
    void HandleStaticGroupCCIdp(idp_t comm, idp_t idp);
    idp_t GetStaticGroupCCIdp(idp_t comm);

    void SynchronizeStaticGroup(idp_t comm);
    void HandleSynchronizeStaticGroup(idp_t comm);

    std::string SearchResource(idp_t idp);
    void HandleSearchResource(idp_t idp, std::string ctrl);
    void HandleSearchResourceInfo(idp_t idp, std::string info);

    ResourceManager() = delete;
    ResourceManager(ResourceManager const&) = delete;
    ResourceManager& operator=(ResourceManager const&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

protected:
    // Retorna o idp global
    idp_t ResolveIdp(idp_t idp);

    // interface to ConsistencyObject, ACABAR AQUI
    void DeallocateResource(idp_t idp);

    // void SendReplica(idp_t idp, Resource *rsc, std::string const& requester);

    void RequestReleaseReplica(idp_t idp);

    void RequestUpdate(idp_t idp);
    // void ReplyUpdate(idp_t idp, Resource *rsc, std::string const& requester);

    void RequestInvalidate(idp_t idp);

    void RequestTokenUpdate(idp_t idp);
    // void ReplyTokenUpdate(idp_t idp, Resource *rsc, std::string const& requester);

    void SendTokenAck(idp_t idp, std::string const& replier);



private:
    void JoinResourceGroup(idp_t idp);
    void LeaveResourceGroup(idp_t idp);

    template <typename T>
    std::unique_ptr<T> AllocateResource(idp_t idp, idp_t ctx, std::string const& name, std::unique_ptr<T> rsc, std::string const& ctrl);

    idp_t GenerateIdp();
    
    void AttachResource(idp_t ctx, hpx::id_type ctx_gid, idp_t idp, std::string const& name);
    hpx::id_type AttachResourceRemote(hpx::id_type ctx_gid, idp_t idp, std::string const& name);

    // to improve
    //void DummyInsertWorldContext(idp_t idp, std::string const& name, Resource *rsc, std::string const& ctrl);

    Controller *_ctrl;

    bool _is_main_mgr;

    std::mutex _mtx;

    std::map<idp_t, hpx::id_type> gids; // equivalente ao _cst_objs

    // std::map<idp_t, ConsistencyObject*> _cst_objs;
    std::map<idp_t, idp_t> _predecessors; // o segundo é o predecessor do primeiro
    std::map<idp_t, idp_t> _alias; // Associação entre

    std::map<idp_t, idp_t> _cache_preds;

    std::map<idp_t, std::condition_variable> _sync_gfind;
    std::map<idp_t, std::condition_variable> _sync_pred;
    std::map<idp_t, std::condition_variable> _sync_replicas;
    std::map<idp_t, std::condition_variable> _sync_free;

    // search resource
    std::map<idp_t, std::tuple<unsigned int, unsigned int, std::string>> _sr_vars;
    std::map<idp_t, std::condition_variable> _sr_cv;

    // static group
    std::map<idp_t, std::pair<unsigned int, unsigned int>> _sg_vars;
    std::map<idp_t, idp_t> _sg_cc;
    std::map<idp_t, std::condition_variable> _sg_cv;

    std::set<idp_t> dynamicOrganizer_idps;
    std::set<idp_t> staticOrganizer_idps;
};

}

#include "cor/services/resource_manager.tpp"

#endif
