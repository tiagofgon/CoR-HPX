#ifndef COR_HPX_HPP
#define COR_HPX_HPP

#include "cor/system/system.hpp"
#include <hpx/hpx.hpp>

#include "cor/system/macros.hpp"

#include "cor/message.hpp"

#include "cor/resources/domain_client.hpp"
#include "cor/resources/closure_client.hpp"
#include "cor/resources/proto_agent_client.hpp"
#include "cor/resources/group_client.hpp"
#include "cor/resources/data_client.hpp"
#include "cor/resources/barrier_client.hpp"
#include "cor/resources/mutex_client.hpp"
#include "cor/resources/rwmutex_client.hpp"
#include "cor/resources/agent_client.hpp"

typedef char *arg[];
typedef std::vector<char> data_type;
typedef void* tipo1;
typedef std::shared_ptr<void> tipo2;
typedef std::shared_ptr<const void> tipo3;

HPX_REGISTER_CHANNEL(data_type);
HPX_REGISTER_CHANNEL(idp_t);

REGISTER_AGENT(idp_t, idp_t);
REGISTER_AGENT(void, int);
REGISTER_AGENT(void, idp_t);
REGISTER_AGENT(void, arg);
REGISTER_AGENT(void, int, arg);

REGISTER_PROTOAGENT(idp_t, idp_t);
REGISTER_PROTOAGENT(void, int);
REGISTER_PROTOAGENT(void, idp_t);
REGISTER_PROTOAGENT(int, int, int);
REGISTER_PROTOAGENT(void, tipo1);
REGISTER_PROTOAGENT(void, tipo2);
REGISTER_PROTOAGENT(void, tipo3);
REGISTER_PROTOAGENT(void);


REGISTER_DATA(int);
REGISTER_DATA(idp_t);

namespace cor {

    // hpx::id_type Initialize_hpx(std::string const& app_group, std::string const& context, unsigned int npods, std::string const& module);
    std::unique_ptr<Domain_Client> Initialize_hpx(std::string const& app_group, std::string const& context, unsigned int npods, std::string const& module);
    void Finalize_hpx();

    std::shared_ptr<Domain_Client> GetDomain();

}

#endif
