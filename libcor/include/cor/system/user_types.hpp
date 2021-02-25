#ifndef COR_DEFINITIONS_HPP
#define COR_DEFINITIONS_HPP

#include <hpx/hpx.hpp>
#include "cor/system/system.hpp"

#include "cor/resources/domain_client.hpp"
#include "cor/resources/closure_client.hpp"
#include "cor/resources/proto_agent_client.hpp"
#include "cor/resources/group_client.hpp"
#include "cor/resources/data_client.hpp"
#include "cor/resources/barrier_client.hpp"
#include "cor/resources/mutex_client.hpp"
#include "cor/resources/rwmutex_client.hpp"
#include "cor/resources/agent_client.hpp"
#include "cor/resources/operon_client.hpp"
#include "cor/resources/duplex_channel_client.hpp"
#include "cor/resources/multi_channel_client.hpp"
#include "cor/resources/uni_channel_client.hpp"


typedef char *arg[];
typedef std::vector<char> data_type;
typedef void* tipo1;
typedef std::shared_ptr<void> tipo2;
typedef std::shared_ptr<const void> tipo3;
typedef std::vector<std::string> tipo4;
typedef std::string string;
typedef std::vector<double> data_type2;


// Agent types
REGISTER_AGENT(idp_t, idp_t);
REGISTER_AGENT(void, int);
REGISTER_AGENT(void, idp_t);
REGISTER_AGENT(void, double);
REGISTER_AGENT(void, char);
REGISTER_AGENT(void, arg);
REGISTER_AGENT(void, int, arg);
REGISTER_AGENT(void, tipo4);
REGISTER_AGENT(void, size_t, size_t, size_t, size_t);


// ProtoAgent types
REGISTER_PROTOAGENT(idp_t, idp_t);
REGISTER_PROTOAGENT(void, int);
REGISTER_PROTOAGENT(void, idp_t);
REGISTER_PROTOAGENT(int, int, int);
REGISTER_PROTOAGENT(void, tipo1);
REGISTER_PROTOAGENT(void, tipo2);
REGISTER_PROTOAGENT(void, tipo3);
REGISTER_PROTOAGENT(void);
REGISTER_PROTOAGENT(void, size_t, size_t, size_t, size_t);


// Data types
REGISTER_DATA(int);
REGISTER_DATA(idp_t);
REGISTER_DATA(double);
REGISTER_DATA(string);


// Channel types
HPX_REGISTER_CHANNEL(int)
HPX_REGISTER_CHANNEL(double)
HPX_REGISTER_CHANNEL(data_type);
HPX_REGISTER_CHANNEL(idp_t);
HPX_REGISTER_CHANNEL(data_type2);

// DuplexChannel types
REGISTER_DUPLEXCHANNEL(data_type2)


// MultiChannel types
REGISTER_MULTICHANNEL(int)
REGISTER_MULTICHANNEL(double)
REGISTER_MULTICHANNEL(data_type2)


// UniChannel types
REGISTER_UNICHANNEL(int)
REGISTER_UNICHANNEL(double)
REGISTER_UNICHANNEL(data_type2)

#endif