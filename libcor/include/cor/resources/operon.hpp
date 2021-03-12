#ifndef COR_OPERON_HPP
#define COR_OPERON_HPP

#include <functional>

#include "cor/resources/resource.hpp"
#include "cor/elements/executor_pool.hpp"
#include "cor/elements/mailbox_client.hpp"

#include <hpx/hpx.hpp>


namespace cor {


struct Operon: public Resource,  public hpx::components::component_base<Operon>
{

typedef typename hpx::components::component_base<Operon>::wrapping_type wrapping_type;
typedef Operon type_holder;
typedef Resource base_type_holder;


public:
    Operon() = delete;
    ~Operon();
    Operon(idp_t idp, std::size_t num_hpx_threads);

    int GetRank();
    int GetNumThreads();

    std::pair<int,int> ScheduleStatic(int Beg, int End);
    std::vector<std::pair<int,int>> ScheduleStatic(int Beg, int End, int chunk);
    std::pair<int,int> ScheduleDynamic(int Beg, int End, int chunk);
    std::pair<int,int> ScheduleGuided(int Beg, int End, int chunk);

    template < typename ... Args >
    void Dispatch(hpx::function<void(Args...)> func, Args ... args);

    void Dispatch_void(hpx::function<void()> func);

    template < typename Func, typename ... Args >
    void Dispatch(Func&& func, Args&&... args);

    template < typename Func>
    void Dispatch(Func&& func);

    void Wait();

    HPX_DEFINE_COMPONENT_ACTION(Operon, GetRank, GetRank_action_Operon); 
    HPX_DEFINE_COMPONENT_ACTION(Operon, GetNumThreads, GetNumThreads_action_Operon);  
    HPX_DEFINE_COMPONENT_ACTION(Operon, Dispatch_void, Dispatch_void_action_Operon);

    template < typename ... Args >
    struct Dispatch_action_Operon
    : hpx::actions::make_action<
        decltype(&Operon::Dispatch<Args...>),
        &Operon::Dispatch<Args...>
    >::type
    {};


    /* Mailbox's interface */
    void Send1(idp_t dest, Message const& msg);                          // Unicast
    void Send2(std::vector<idp_t> const& dests, Message const& msg);
    Message Receive1();
    Message Receive2(idp_t source);

    // Contextual Communication
    void Broadcast(idp_t clos, Message const& msg);                     // Broadcast
    void Send3(idm_t rank, idp_t clos, Message const& msg);              // Contextual Unicast
    Message Receive3(idm_t rank, idp_t clos);

    hpx::id_type GetMailboxGid();

    HPX_DEFINE_COMPONENT_ACTION(Operon, Send1, Send1_action_Operon);
    HPX_DEFINE_COMPONENT_ACTION(Operon, Send2, Send2_action_Operon);
    HPX_DEFINE_COMPONENT_ACTION(Operon, Receive1, Receive1_action_Operon);
    HPX_DEFINE_COMPONENT_ACTION(Operon, Receive2, Receive2_action_Operon);
    HPX_DEFINE_COMPONENT_ACTION(Operon, Broadcast, Broadcast_action_Operon);
    HPX_DEFINE_COMPONENT_ACTION(Operon, Send3, Send3_action_Operon);
    HPX_DEFINE_COMPONENT_ACTION(Operon, Receive3, Receive3_action_Operon);
    HPX_DEFINE_COMPONENT_ACTION(Operon, GetMailboxGid, GetMailboxGid_action_Operon);

private:
    ExecutorPool _executor_pool;
    Mailbox_Client _mailBox;
};

}


typedef cor::Operon::GetNumThreads_action_Operon GetNumThreads_action_Operon;
HPX_REGISTER_ACTION_DECLARATION(GetNumThreads_action_Operon);


typedef cor::Operon::Dispatch_void_action_Operon Dispatch_void_action_Operon;

HPX_REGISTER_ACTION_DECLARATION(Dispatch_void_action_Operon);


typedef cor::Operon::GetRank_action_Operon GetRank_action_Operon;
typedef cor::Operon::Send1_action_Operon Send1_action_Operon;
typedef cor::Operon::Send2_action_Operon Send2_action_Operon;
typedef cor::Operon::Receive1_action_Operon Receive1_action_Operon;
typedef cor::Operon::Receive2_action_Operon Receive2_action_Operon;
typedef cor::Operon::Broadcast_action_Operon Broadcast_action_Operon;
typedef cor::Operon::Send3_action_Operon Send3_action_Operon;
typedef cor::Operon::Receive3_action_Operon Receive3_action_Operon;
typedef cor::Operon::GetMailboxGid_action_Operon GetMailboxGid_action_Operon;

HPX_REGISTER_ACTION_DECLARATION(GetRank_action_Operon);
HPX_REGISTER_ACTION_DECLARATION(Send1_action_Operon);
HPX_REGISTER_ACTION_DECLARATION(Send2_action_Operon);
HPX_REGISTER_ACTION_DECLARATION(Receive1_action_Operon);
HPX_REGISTER_ACTION_DECLARATION(Receive2_action_Operon);
HPX_REGISTER_ACTION_DECLARATION(Broadcast_action_Operon);
HPX_REGISTER_ACTION_DECLARATION(Send3_action_Operon);
HPX_REGISTER_ACTION_DECLARATION(Receive3_action_Operon);
HPX_REGISTER_ACTION_DECLARATION(GetMailboxGid_action_Operon);


#include "cor/resources/operon.tpp"

#endif
