#ifndef COR_STATIC_ORGANIZER_HPP
#define COR_STATIC_ORGANIZER_HPP

#include "cor/system/macros.hpp"

#include <hpx/hpx.hpp>


namespace cor {

struct StaticOrganizer
{

friend class hpx::serialization::access;
friend class Closure;

protected:
    StaticOrganizer();
    explicit StaticOrganizer(idp_t idp, unsigned int total_members, idp_t parent);

public:
    ~StaticOrganizer ();

    // StaticOrganizer(const StaticOrganizer&) = delete;
    // StaticOrganizer& operator=(const StaticOrganizer&) = delete;

    // StaticOrganizer(StaticOrganizer&&) noexcept;
    // StaticOrganizer& operator=(StaticOrganizer&&) noexcept;

    void Join(idp_t idp, std::string const& name);
    void Leave(idp_t idp);

    idp_t GetParent() const;
    std::size_t GetTotalMembers();
    std::size_t GetFixedTotalMembers() const;
    std::vector<idp_t> GetMemberList();

    idp_t GetIdp(idm_t idm);
    idp_t GetIdp(std::string const& name);

    idm_t GetIdm(idp_t idp);
    idm_t GetIdm(std::string const& name);

    idp_t GetStaticOrganizerIdp() const;


private:
	template <typename Archive>
	void serialize(Archive& ar, unsigned) {
        ar & _idp;
        ar & _total_members;
        ar & _parent;
        ar & _members;
        ar & _next_idm;
	}

    idp_t _idp;
    unsigned int _total_members;
    idp_t _parent;
    std::map<idp_t, std::pair<idm_t, std::string>> _members; // (idp (idm, name))
    idm_t _next_idm;

    hpx::lcos::local::shared_mutex _mtx;
};

}


#endif
