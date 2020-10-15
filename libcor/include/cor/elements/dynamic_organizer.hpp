#ifndef COR_DYNAMIC_ORGANIZER_HPP
#define COR_DYNAMIC_ORGANIZER_HPP

#include <string>
#include <map>
#include <vector>
#include <hpx/hpx.hpp>

#include "cor/system/macros.hpp"

namespace cor {

class DynamicOrganizer
{

friend class hpx::serialization::access;
friend class Domain;
friend class Group;
friend class Teste;

public:
    ~DynamicOrganizer();

    // DynamicOrganizer(const DynamicOrganizer&) = delete;
    // DynamicOrganizer& operator=(const DynamicOrganizer&) = delete;

    // DynamicOrganizer(DynamicOrganizer&&) noexcept;
    // DynamicOrganizer& operator=(DynamicOrganizer&&) noexcept;

    void Join(idp_t idp, std::string const& name);
    void Leave(idp_t idp);

    std::string GetModuleName() const;
    
    std::size_t GetTotalMembers() const;
    std::vector<idp_t> GetMemberList() const;

    idp_t GetIdp(idm_t idm) const;
    idp_t GetIdp(std::string const& name) const;

    idm_t GetIdm(idp_t idp) const;
    idm_t GetIdm(std::string const& name) const;

    idp_t GetDynamicIdp() const;

protected:
    DynamicOrganizer();
    explicit DynamicOrganizer(idp_t idp, std::string const& module);

private:
	template <typename Archive>
	void serialize(Archive& ar, unsigned) {
        ar & _idp;
        ar & _module;
        ar & _members;
        ar & _next_idm;
		// std::cout << "serialized\n";
	}

    idp_t _idp;
    std::string _module;
    std::map<idp_t, std::pair<idm_t, std::string>> _members; // (idp (idm, nome))
    idm_t _next_idm;
    
};

}

#endif
