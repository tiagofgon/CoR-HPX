#ifndef COR_CONTAINER_HPP
#define COR_CONTAINER_HPP

#include "cor/system/macros.hpp"

#include <hpx/hpx.hpp>


namespace cor {

class Container
{

friend class hpx::serialization::access;
friend class Domain;

protected:
    Container();
    explicit Container(idp_t idp);

public:
    ~Container();

    // Container(const Container&) = delete;
    // Container& operator=(const Container&) = delete;

    // Container(Container&&) noexcept;
    // Container& operator=(Container&&) noexcept;

    std::string GetGlobalContext();
    std::string GetLocalContext();

    unsigned int GetTotalPods();
    unsigned int GetTotalDomains();

    // now receives the id parameter to propagate the id of the active thread from whoever invoked this function inside the module, necessary because the active thread changes from component to component
    idp_t GetActiveResourceIdp(size_t id);
    idp_t GetPredecessorIdp(idp_t idp);

    template <typename T>
    std::unique_ptr<T> GetLocalResource(idp_t idp);

    template <typename T>
    std::unique_ptr<T> CreateLocal_agent(idp_t ctx, std::string const& name, hpx::function<void(int)> const& func);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T, typename ... Args>
    idp_t Create(idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T, typename ... Args>
    idp_t CreateRemote(idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T>
    std::unique_ptr<T> CreateReference(idp_t idp, idp_t ctx, std::string const& name);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args);

    // now receives the active_rsc_idp parameter to propagate the idp of the active thread from those who invoked this function in the module, which will be necessary in the pod
    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective(idp_t active_rsc_idp, idp_t clos, idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T, typename ... Args>
    void Run(idp_t idp, Args&&... args);

    template <typename T>
    void Wait(idp_t idp);

    template <typename T>
    auto Get(idp_t idp);

    idp_t Spawn(std::string const& context, unsigned int npods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts);

    idp_t GetContainerIdp() const;


private:
	template <typename Archive>
	void serialize(Archive& ar, unsigned) {
        ar & _idp;
	}

    idp_t _idp;

};


}


#include "cor/elements/container.tpp"


#endif
