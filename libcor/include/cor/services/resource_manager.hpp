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


    std::string SearchResource(idp_t idp);
    
    ResourceManager() = delete;
    ResourceManager(ResourceManager const&) = delete;
    ResourceManager& operator=(ResourceManager const&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

protected:
    // Retorna o idp global
    idp_t ResolveIdp(idp_t idp);

    // Only for local resources
    void DeallocateResource(idp_t idp);

    void RequestInvalidate(idp_t idp);


private:
    template <typename T>
    std::unique_ptr<T> AllocateResource(idp_t idp, idp_t ctx, std::string const& name, std::unique_ptr<T> rsc, std::string const& ctrl);

    idp_t GenerateIdp();
    
    void AttachResource(idp_t ctx, hpx::id_type ctx_gid, idp_t idp, std::string const& name);
    hpx::id_type AttachResourceRemote(hpx::id_type ctx_gid, idp_t idp, std::string const& name);

    Controller *_ctrl;

    bool _is_main_mgr;

    std::mutex _mtx;

    std::map<idp_t, hpx::id_type> gids; // equivalente ao _cst_objs

    std::map<idp_t, idp_t> _predecessors; // o segundo é o predecessor do primeiro
    std::map<idp_t, idp_t> _alias; // Associação entre


    std::set<idp_t> dynamicOrganizer_idps;
    std::set<idp_t> staticOrganizer_idps;
};

}

#include "cor/services/resource_manager.tpp"

#endif
