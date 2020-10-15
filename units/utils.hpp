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

template <typename T>
void AgentRange(std::size_t& begin, std::size_t& end)
{
    std::size_t l_begin, l_end;
    std::size_t size, d, r;

    auto rank = GetRank<T>() + 1;
    auto num_agents = GetSize<T>();

    size = end - begin;
    d = size / num_agents;
    r = size % num_agents;

    l_end = begin;
    for (std::size_t i = 1; i <= rank; ++i) {
        l_begin = l_end;
        l_end = l_begin + d;
        if (r) {
            l_end++;
            r--;
        }
    }

    begin = l_begin;
    end = l_end;
}

#endif
