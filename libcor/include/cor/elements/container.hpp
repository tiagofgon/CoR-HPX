#ifndef COR_CONTAINER_HPP
#define COR_CONTAINER_HPP

#include "cor/system/macros.hpp"

#include <hpx/hpx.hpp>

namespace cor {

class Container
{

friend class hpx::serialization::access;
friend class Domain;
friend class Teste;

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

    // agora recebe o parametro id para propagar o id da thread ativa de quem invocou esta função no módulo, necessário porque a thread ativa troca de componente em componente
    idp_t GetActiveResourceIdp(size_t id);
    idp_t GetPredecessorIdp(idp_t idp);

    template <typename T>
    std::unique_ptr<T> GetLocalResource(idp_t idp);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateLocal(idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T, typename ... Args>
    idp_t CreateRemote(idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T, typename ... Args>
    idp_t Create(idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T>
    std::unique_ptr<T> CreateReference(idp_t idp, idp_t ctx, std::string const& name);

    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective(idp_t ctx, std::string const& name, unsigned int total_members, Args&& ... args);

    // agora recebe o parametro active_rsc_idp para propagar o idp da thread ativa de quem invocou esta função no módulo, que irá ser necessário no pod
    template <typename T, typename ... Args>
    std::unique_ptr<T> CreateCollective(idp_t active_rsc_idp, idp_t clos, idp_t ctx, std::string const& name, Args&& ... args);

    template <typename T, typename ... Args>
    void Run(idp_t idp, Args&&... args);

    template <typename T>
    void Wait(idp_t idp);

    template <typename T>
    auto Get(idp_t idp);

    idp_t Spawn(std::string const& context, unsigned int npods, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts);

    idp_t GetContainerIdp() const;

    template <typename T>
    void Migrar(hpx::id_type gid, hpx::id_type dest);

protected:
    Container();
    explicit Container(idp_t idp);

private:
	template <typename Archive>
	void serialize(Archive& ar, unsigned) {
        ar & _idp;
		// std::cout << "serialized\n";
	}

    idp_t _idp;

};

}

#include "cor/elements/container.tpp"

#endif
