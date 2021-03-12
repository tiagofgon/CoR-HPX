#ifndef MODULES_UTILS_HPP
#define MODULES_UTILS_HPP

#include "cor/cor.hpp"

template <typename T>
idm_t GetRank(idp_t rsc_idp)
{
    auto domain = cor::GetDomain();
    auto organizer_idp = domain->GetPredecessorIdp(rsc_idp);
    auto organizer = domain->GetLocalResource<T>(organizer_idp);
    return organizer->GetIdm(rsc_idp);
}

template <typename T>
idm_t GetRank()
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    return GetRank<T>(agent_idp);
}

template <typename T>
std::size_t GetSize(idp_t organizer_idp)
{
    auto domain = cor::GetDomain();
    auto organizer = domain->GetLocalResource<T>(organizer_idp);
    return organizer->GetTotalMembers();
}

template <typename T>
std::size_t GetSize()
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto organizer_idp = domain->GetPredecessorIdp(agent_idp);
    return GetSize<T>(organizer_idp);
}

#endif
