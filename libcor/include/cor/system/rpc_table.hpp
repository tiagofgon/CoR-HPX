// #ifndef COR_RPC_TABLE_HPP
// #define COR_RPC_TABLE_HPP

// CZRPC_ALLOW_CONST_LVALUE_REFS;
// CZRPC_ALLOW_NON_CONST_LVALUE_REFS;
// CZRPC_ALLOW_RVALUE_REFS;

// #include "cor/resources/agent.hpp"
// #include "cor/resources/proto_agent.hpp"
// #include "cor/resources/barrier.hpp"
// #include "cor/resources/closure.hpp"
// #include "cor/resources/data.hpp"
// #include "cor/resources/domain.hpp"
// #include "cor/resources/group.hpp"
// #include "cor/resources/mutex.hpp"
// #include "cor/resources/rwmutex.hpp"

// #include "cor/system/rpc_class.hpp"

// #define CREATE(...) REGISTERRPC(__VA_ARGS__)
// #define RUN(...) REGISTERRPC(__VA_ARGS__)
// #define WAIT(...) REGISTERRPC(__VA_ARGS__)
// #define GET(...) REGISTERRPC(__VA_ARGS__)

// #define RPCTABLE_CLASS cor::RPC
// #define RPCTABLE_CONTENTS                                                                                                           \
//     CREATE(Group, group, Create<cor::Group, std::string const&>)                                                                    \
//     CREATE(Data<std::vector<int>>, data, Create<cor::Data<std::vector<int>>, std::vector<int>>)                                     \
//     CREATE(Agent<void()>, agent, Create<cor::Agent<void()>, std::string const&, std::string const&>)                                \
//     CREATE(Agent<idp_t()>, agent1, Create<cor::Agent<idp_t()>, std::string const&, std::string const&>)                             \
//     CREATE(Agent<void(idp_t)>, agent2, Create<cor::Agent<void(idp_t)>, std::string const&, std::string const&>)                     \
//     CREATE(Agent<idp_t(idp_t)>, agent3, Create<cor::Agent<idp_t(idp_t)>, std::string const&, std::string const&>)                   \
//     CREATE(ProtoAgent<void()>, protoagent, Create<cor::ProtoAgent<void()>, std::string const&, std::string const&>)                 \
//     CREATE(ProtoAgent<idp_t()>, protoagent1, Create<cor::ProtoAgent<idp_t()>, std::string const&, std::string const&>)              \
//     CREATE(ProtoAgent<void(idp_t)>, protoagent2, Create<cor::ProtoAgent<void(idp_t)>, std::string const&, std::string const&>)      \
//     CREATE(ProtoAgent<idp_t(idp_t)>, protoagent3, Create<cor::ProtoAgent<idp_t(idp_t)>, std::string const&, std::string const&>)    \
//                                                                                                                                     \
//     RUN(Executor<void()>, run, Run<void()>)                                                                                         \
//     RUN(Executor<idp_t()>, run1, Run<idp_t()>)                                                                                      \
//     RUN(Executor<void(idp_t)>, run2, Run<void(idp_t), idp_t>)                                                                       \
//     RUN(Executor<idp_t(idp_t)>, run3, Run<idp_t(idp_t), idp_t>)                                                                     \
//                                                                                                                                     \
//     WAIT(Executor<void()>, wait, Wait<void()>)                                                                                      \
//     WAIT(Executor<idp_t()>, wait1, Wait<idp_t()>)                                                                                   \
//     WAIT(Executor<void(idp_t)>, wait2, Wait<void(idp_t)>)                                                                           \
//     WAIT(Executor<idp_t(idp_t)>, wait3, Wait<idp_t(idp_t)>)                                                                         \
//                                                                                                                                     \
//     GET(Executor<void()>, get, Get<void, void()>)                                                                                   \
//     GET(Executor<idp_t()>, get1, Get<idp_t, idp_t()>)                                                                               \
//     GET(Executor<void(idp_t)>, get2, Get<void, void(idp_t)>)                                                                        \
//     GET(Executor<idp_t(idp_t)>, get3, Get<idp_t, idp_t(idp_t)>)

// #include "cor/external/crazygaze/rpc/RPCGenerate.h"

// namespace cor {

//     template <typename T, typename ENABLED = void>
//     struct ResourceTraits {
//         using rsc_type = T;
//         static constexpr bool valid = false;
//     };

//     template <typename T, typename ENABLED = void>
//     struct RunTraits {
//         using rsc_type = T; //type
//         static constexpr bool valid = false;
//     };

//     template <typename T, typename ENABLED = void>
//     struct WaitTraits {
//         using rsc_type = T; //type
//         static constexpr bool valid = false;
//     };

//     template <typename T, typename ENABLED = void>
//     struct GetTraits {
//         using rsc_type = T; //type
//         static constexpr bool valid = false;
//     };

// }

// #undef CREATE
// #define CREATE(rsc, id, ...)                                                                                            \
//     namespace cor {                                                                                                     \
//         template <>                                                                                                     \
//         struct ResourceTraits<rsc> {                                                                                    \
//             using rsc_type = rsc;                                                                                       \
//             using func_type = decltype(&RPCTABLE_CLASS::__VA_ARGS__);                                                   \
//             static constexpr cz::rpc::Table<RPCTABLE_CLASS>::RPCId rpcid = cz::rpc::Table<RPCTABLE_CLASS>::RPCId::id;   \
//             static constexpr bool valid = true;                                                                         \
//         };                                                                                                              \
//     }

// #undef RUN
// #define RUN(rsc, id, ...)                                                                                               \
//     namespace cor {                                                                                                     \
//         template <>                                                                                                     \
//         struct RunTraits<rsc> {                                                                                         \
//             using rsc_type = rsc;                                                                                       \
//             using func_type = decltype(&RPCTABLE_CLASS::__VA_ARGS__);                                                   \
//             static constexpr cz::rpc::Table<RPCTABLE_CLASS>::RPCId rpcid = cz::rpc::Table<RPCTABLE_CLASS>::RPCId::id;   \
//             static constexpr bool valid = true;                                                                         \
//         };                                                                                                              \
//     }

// #undef WAIT
// #define WAIT(rsc, id, ...)                                                                                              \
//     namespace cor {                                                                                                     \
//         template <>                                                                                                     \
//         struct WaitTraits<rsc> {                                                                                        \
//             using rsc_type = rsc;                                                                                       \
//             using func_type = decltype(&RPCTABLE_CLASS::__VA_ARGS__);                                                   \
//             static constexpr cz::rpc::Table<RPCTABLE_CLASS>::RPCId rpcid = cz::rpc::Table<RPCTABLE_CLASS>::RPCId::id;   \
//             static constexpr bool valid = true;                                                                         \
//         };                                                                                                              \
//     }

// #undef GET
// #define GET(rsc, id, ...)                                                                                               \
//     namespace cor {                                                                                                     \
//         template <>                                                                                                     \
//         struct GetTraits<rsc> {                                                                                         \
//             using rsc_type = rsc;                                                                                       \
//             using func_type = decltype(&RPCTABLE_CLASS::__VA_ARGS__);                                                   \
//             static constexpr cz::rpc::Table<RPCTABLE_CLASS>::RPCId rpcid = cz::rpc::Table<RPCTABLE_CLASS>::RPCId::id;   \
//             static constexpr bool valid = true;                                                                         \
//         };                                                                                                              \
//     }

// RPCTABLE_CONTENTS;

// #undef REGISTERRPC
// #undef RPCTABLE_CLASS
// #undef RPCTABLE_CONTENTS

// #endif
