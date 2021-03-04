#include "cor/elements/container.hpp"


namespace cor {

Container::Container() = default;
Container::~Container() = default;

Container::Container(idp_t idp, unsigned int pod_id) : 
    _idp{idp},
    _pod_id{pod_id}
{
    // std::cout << "Criado um objeto da classe \"Container\", com idp: " << _idp << std::endl;
}



// Container::Container(Container&&) noexcept = default;

// Container& Container::operator=(Container&&) noexcept = default;

std::string Container::GetGlobalContext()
{
    return global::pods[_pod_id]->GetGlobalContext();
}

std::string Container::GetLocalContext()
{
    return global::pods[_pod_id]->GetLocalContext();
}

unsigned int Container::GetNumPods()
{
    return global::pods[_pod_id]->GetNumPods();
}

unsigned int Container::GetNumDomains()
{
    return global::pods[_pod_id]->GetNumDomains();
}

std::vector<idp_t> Container::GetPods()
{
    return global::pods[_pod_id]->GetPods();
}

std::vector<idp_t> Container::GetDomains()
{
    return global::pods[_pod_id]->GetDomains();
}

idp_t Container::GetActiveResourceIdp(size_t id)
{
    return global::pods[_pod_id]->GetActiveResourceIdp2(id);
}

idp_t Container::GetPredecessorIdp(idp_t idp)
{
    return global::pods[_pod_id]->GetPredecessorIdp(idp);
}

idp_t Container::Spawn(std::string const& context, unsigned int npods, unsigned int total_pods, idp_t parent, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
{
    return global::pods[_pod_id]->Spawn(context, npods, total_pods, parent, module, args, hosts);   
}

idp_t Container::GetContainerIdp() const
{
    return _idp;
}


}
