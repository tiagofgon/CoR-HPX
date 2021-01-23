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





// class MyObject
// {
// public:
//     friend class hpx::serialization::access;

//     MyObject() = default;
//     ~MyObject() = default;

//     void setName(std::string name) {
//         this->name = name;
//     }
//     void setId(size_t id) {
//         this->id = id;
//     }
//     std::string getName() {
//         return name;
//     }
//     size_t getId() {
//         return id;
//     }
// private:
//     std::string name;
//     size_t id;

// 	template <typename Archive>
// 	void serialize(Archive& ar, unsigned) {
//         ar & id;
//         ar & name;
// 	}
// };


typedef char *arg[];
typedef std::vector<char> data_type;
typedef void* tipo1;
typedef std::shared_ptr<void> tipo2;
typedef std::shared_ptr<const void> tipo3;
typedef std::vector<std::string> tipo4;
typedef std::string string;

HPX_REGISTER_CHANNEL(data_type);
HPX_REGISTER_CHANNEL(idp_t);

REGISTER_AGENT(idp_t, idp_t);
REGISTER_AGENT(void, int);
REGISTER_AGENT(void, idp_t);
REGISTER_AGENT(void, double);
REGISTER_AGENT(void, char);
REGISTER_AGENT(void, arg);
REGISTER_AGENT(void, int, arg);
REGISTER_AGENT(void, tipo4);
REGISTER_AGENT(void, size_t, size_t, size_t, size_t);

REGISTER_PROTOAGENT(idp_t, idp_t);
REGISTER_PROTOAGENT(void, int);
REGISTER_PROTOAGENT(void, idp_t);
REGISTER_PROTOAGENT(int, int, int);
REGISTER_PROTOAGENT(void, tipo1);
REGISTER_PROTOAGENT(void, tipo2);
REGISTER_PROTOAGENT(void, tipo3);
REGISTER_PROTOAGENT(void);
REGISTER_PROTOAGENT(void, size_t, size_t, size_t, size_t);

REGISTER_DATA(int);
REGISTER_DATA(idp_t);
REGISTER_DATA(double);
REGISTER_DATA(string);

#endif