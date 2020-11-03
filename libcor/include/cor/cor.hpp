#ifndef COR_HPX_HPP
#define COR_HPX_HPP

// #include <hpx/hpx.hpp>

// #include <hpx/hpx.hpp>


// #include <hpx/hpx.hpp>

// #include "cor/resources/domain.hpp"
// #include <string>



// #include "cor/system/system.hpp"

// #include "cor/system/macros.hpp"
// // #include <hpx/hpx.hpp>





// #include <string>

#include "cor/system/system.hpp"
#include <hpx/hpx.hpp>

#include "cor/system/macros.hpp"


#include "cor/resources/domain_client.hpp"
#include "cor/resources/closure_client.hpp"
#include "cor/resources/proto_agent_client.hpp"
#include "cor/resources/group_client.hpp"
#include "cor/resources/data_client.hpp"
#include "cor/resources/barrier_client.hpp"
#include "cor/resources/mutex_client.hpp"
#include "cor/resources/teste.hpp"
#include "cor/resources/teste_empty.hpp"

REGISTER_PROTOAGENT(idp_t, idp_t);
REGISTER_PROTOAGENT(void, int);
REGISTER_PROTOAGENT(void, idp_t);
REGISTER_DATA(int);
REGISTER_DATA(idp_t);

namespace cor {

    // hpx::id_type Initialize_hpx(std::string const& app_group, std::string const& context, unsigned int npods, std::string const& module);
    std::unique_ptr<Domain_Client> Initialize_hpx(std::string const& app_group, std::string const& context, unsigned int npods, std::string const& module);
    void Finalize_hpx();

    std::shared_ptr<Domain_Client> GetDomain();

}

#endif
