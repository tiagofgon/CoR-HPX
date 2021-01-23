#ifndef COR_HPX_HPP
#define COR_HPX_HPP

#include "cor/system/system.hpp"
#include <hpx/hpx.hpp>

#include "cor/system/user_types.hpp"


namespace cor {

    // hpx::id_type Initialize_hpx(std::string const& app_group, std::string const& context, unsigned int npods, std::string const& module);
    std::unique_ptr<Domain_Client> Initialize_hpx(std::string const& app_group, std::string const& context, unsigned int npods, std::string const& module);
    void Finalize_hpx();

    hpx::future<std::shared_ptr<Domain_Client>> GetDomain(hpx::launch::async_policy);
    std::shared_ptr<Domain_Client> GetDomain();

}

#endif
