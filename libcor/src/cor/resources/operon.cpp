#include "cor/resources/operon.hpp"

namespace cor {

Operon::~Operon() = default;

Operon::Operon(idp_t idp, std::size_t num_hpx_threads) :
    Resource{idp},
    _executor_pool{idp, num_hpx_threads},
    _mailBox{idp}
{
    std::cout << "Criado um componente \"Operon\", com idp: " << idp << ", com threads: " << num_hpx_threads << std::endl;
}


/* Executor_pool's interface */
int Operon::GetRank()
{
    return _executor_pool.GetRank();
}

int Operon::GetNumThreads() {
    return _executor_pool.GetNumThreads();
}

std::pair<int,int> Operon::ScheduleStatic(int Beg, int End)
{
    return _executor_pool.ScheduleStatic(Beg, End);
}

std::vector<std::pair<int,int>> Operon::ScheduleStatic(int Beg, int End, int chunk)
{
    return _executor_pool.ScheduleStatic(Beg, End, chunk);
}

std::pair<int,int> Operon::ScheduleDynamic(int Beg, int End, int chunk)
{
    return _executor_pool.ScheduleDynamic(Beg, End, chunk);
}

std::pair<int,int> Operon::ScheduleGuided(int Beg, int End, int chunk)
{
    return _executor_pool.ScheduleGuided(Beg, End, chunk);
}

void Operon::Dispatch_void(hpx::function<void()> func)
{
    return _executor_pool.Dispatch(func);
}




/* Mailbox's interface */
void Operon::Send1(idp_t dest, Message const& msg)
{
    return _mailBox.Send(dest, msg);
}

void Operon::Send2(std::vector<idp_t> const& dests, Message const& msg)
{
    return _mailBox.Send(dests, msg);
}

Message Operon::Receive1()
{
    return _mailBox.Receive();
}

Message Operon::Receive2(idp_t source)
{
    return _mailBox.Receive(source);
}

void Operon::Broadcast(idp_t clos, Message const& msg)
{
    return _mailBox.Broadcast(clos, msg);
}

void Operon::Send3(idm_t rank, idp_t clos, Message const& msg)
{
    return _mailBox.Send(rank, clos, msg);
}

Message Operon::Receive3(idm_t rank, idp_t clos)
{
    return _mailBox.Receive(rank, clos);
}

hpx::id_type Operon::GetMailboxGid()
{
    return _mailBox.GetGid();
}

}


typedef cor::Operon Operon;
typedef hpx::components::component<Operon> Operon_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(Operon_type, Operon, "Resource");


/* Action registration to interact with DynamicOranizer */
typedef cor::Operon::GetNumThreads_action_Operon GetNumThreads_action_Operon;
typedef cor::Operon::Dispatch_void_action_Operon Dispatch_void_action_Operon;

HPX_REGISTER_ACTION(GetNumThreads_action_Operon);
HPX_REGISTER_ACTION(Dispatch_void_action_Operon);



typedef cor::Operon::Send1_action_Operon Send1_action_Operon;
typedef cor::Operon::Send2_action_Operon Send2_action_Operon;
typedef cor::Operon::Receive1_action_Operon Receive1_action_Operon;
typedef cor::Operon::Receive2_action_Operon Receive2_action_Operon;
typedef cor::Operon::Broadcast_action_Operon Broadcast_action_Operon;
typedef cor::Operon::Send3_action_Operon Send3_action_Operon;
typedef cor::Operon::Receive3_action_Operon Receive3_action_Operon;
typedef cor::Operon::GetMailboxGid_action_Operon GetMailboxGid_action_Operon;

HPX_REGISTER_ACTION(Send1_action_Operon);
HPX_REGISTER_ACTION(Send2_action_Operon);
HPX_REGISTER_ACTION(Receive1_action_Operon);
HPX_REGISTER_ACTION(Receive2_action_Operon);
HPX_REGISTER_ACTION(Broadcast_action_Operon);
HPX_REGISTER_ACTION(Send3_action_Operon);
HPX_REGISTER_ACTION(Receive3_action_Operon);
HPX_REGISTER_ACTION(GetMailboxGid_action_Operon);