#include "cor/elements/container.hpp"


namespace cor {

Container::Container() = default;
Container::~Container() = default;

Container::Container(idp_t idp) : 
    _idp{idp} 
{
    // std::cout << "Criado um objeto da classe \"Container\", com idp: " << _idp << std::endl;
}

// Container::Container(Container&&) noexcept = default;

// Container& Container::operator=(Container&&) noexcept = default;

std::string Container::GetGlobalContext()
{
    return global::pod->GetGlobalContext();
}

std::string Container::GetLocalContext()
{
    return global::pod->GetLocalContext();
}

unsigned int Container::GetTotalPods()
{    
    return global::pod->GetTotalPods();
}

unsigned int Container::GetTotalDomains()
{
    return global::pod->GetTotalDomains();
}

idp_t Container::GetActiveResourceIdp(size_t id)
{
    return global::pod->GetActiveResourceIdp2(id);
}

idp_t Container::GetPredecessorIdp(idp_t idp)
{
    return global::pod->GetPredecessorIdp(idp);
}

idp_t Container::Spawn(std::string const& context, unsigned int npods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
{
    return global::pod->Spawn(context, npods, parent, module, args, hosts);   
}

idp_t Container::GetContainerIdp() const
{
    return _idp;
}


}
