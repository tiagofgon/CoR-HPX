#ifndef COR_STATIC_ORGANIZER_HPP
#define COR_STATIC_ORGANIZER_HPP

#include <string>
#include <map>
#include <vector>
#include <hpx/hpx.hpp>

#include "cor/system/macros.hpp"

namespace cor {

struct StaticOrganizer
{

friend class hpx::serialization::access;
friend class Closure;

public:
    ~StaticOrganizer ();

    // StaticOrganizer(const StaticOrganizer&) = delete;
    // StaticOrganizer& operator=(const StaticOrganizer&) = delete;

    // StaticOrganizer(StaticOrganizer&&) noexcept;
    // StaticOrganizer& operator=(StaticOrganizer&&) noexcept;

    void Join(idp_t idp, std::string const& name);
    void Leave(idp_t idp);

    idp_t GetParent() const;
    std::size_t GetTotalMembers() const;
    // novo metodo para retornar o atributo _total_members, que é o numero fixo de membros deste organizador estático
    std::size_t GetFixedTotalMembers() const;
    std::vector<idp_t> GetMemberList() const;

    idp_t GetIdp(idm_t idm) const;
    idp_t GetIdp(std::string const& name) const;

    idm_t GetIdm(idp_t idp) const;
    idm_t GetIdm(std::string const& name) const;

    idp_t GetStaticIdp() const;

protected:
    StaticOrganizer();
    explicit StaticOrganizer (idp_t idp, unsigned int total_members, idp_t parent);

private:
	template <typename Archive>
	void serialize(Archive& ar, unsigned) {
        ar & _idp;
        ar & _total_members;
        ar & _parent;
        ar & _members;
        ar & _next_idm;
		// std::cout << "serialized\n";
	}

    idp_t _idp;
    unsigned int _total_members;
    idp_t _parent;
    std::map<idp_t, std::pair<idm_t, std::string>> _members;
    idm_t _next_idm;

};

}


#endif
