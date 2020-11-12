#ifndef COR_MAILBOX_HPP
#define COR_MAILBOX_HPP

#include <vector>

#include "cor/system/macros.hpp"
#include "cor/message.hpp"
#include <hpx/hpx.hpp>

#include <queue>
#include <atomic>
#include <functional>
#include <mutex>

typedef std::vector<char> data_type;

namespace cor {

struct Mailbox: public hpx::components::locking_hook< hpx::components::component_base<Mailbox> >
// class Mailbox
{

public:

    Mailbox();
    explicit Mailbox(idp_t idp);
    ~Mailbox();

    // Mailbox(const Mailbox&) = delete;
    // Mailbox& operator=(const Mailbox&) = delete;

    // Mailbox(Mailbox&&) noexcept;
    // Mailbox& operator=(Mailbox&&) noexcept;

    void Send1(idp_t dest, Message const& msg) const;                          // Unicast
    void Send2(std::vector<idp_t> const& dests, Message const& msg) const;     // Multicast
    Message Receive1();
    Message Receive2(idp_t source);
    void Receive_to_map(idp_t source, Message const& msg);

    // Contextual Communication
    void Broadcast(idp_t clos, Message const& msg);                     // Broadcast
    void Send3(idm_t rank, idp_t clos, Message const& msg);              // Contextual Unicast
    Message Receive3(idm_t rank, idp_t clos);

    void SendNewClos(Message const& msg_original);
    idp_t ReceiveNewClos(std::string const& ctx);

    HPX_DEFINE_COMPONENT_ACTION(Mailbox, Send1, Send1_action_Mailbox);
    HPX_DEFINE_COMPONENT_ACTION(Mailbox, Send2, Send2_action_Mailbox);
    HPX_DEFINE_COMPONENT_ACTION(Mailbox, Receive1, Receive1_action_Mailbox);
    HPX_DEFINE_COMPONENT_ACTION(Mailbox, Receive2, Receive2_action_Mailbox);
    HPX_DEFINE_COMPONENT_ACTION(Mailbox, Receive_to_map, Receive_to_map_action_Mailbox);
    HPX_DEFINE_COMPONENT_ACTION(Mailbox, Broadcast, Broadcast_action_Mailbox);
    HPX_DEFINE_COMPONENT_ACTION(Mailbox, Send3, Send3_action_Mailbox);
    HPX_DEFINE_COMPONENT_ACTION(Mailbox, Receive3, Receive3_action_Mailbox);
    HPX_DEFINE_COMPONENT_ACTION(Mailbox, SendNewClos, SendNewClos_action_Mailbox);
    HPX_DEFINE_COMPONENT_ACTION(Mailbox, ReceiveNewClos, ReceiveNewClos_action_Mailbox);


private:
    idp_t _idp;
    hpx::lcos::channel<data_type> _channel;
    std::map<idp_t, std::queue<Message>> _mailboxes;
    std::vector<idp_t> _messages_order;

    hpx::lcos::channel<data_type> _channel_ctx;
    std::map<std::string, idp_t> _mailboxes_ctr; // context -> idp da nova clausura


    // std::map<idp_t, hpx::lcos::channel<data_type>> _channels;
    // std::map<idp_t, std::queue<Message>> _mailboxes;
    // std::map<idp_t, hpx::lcos::local::promise<void>> _promises;
    // std::map<idp_t, hpx::future<void> > _futures;
    // std::map<idp_t, bool> future_constructed;

    // hpx::lcos::local::promise<void> _promise;
    // hpx::future<void> _future;
    // bool future_constructed_general;

    // hpx::lcos::local::spinlock _mtx;
    
    // hpx::lcos::local::mutex _mtx;
    // std::map<idp_t, hpx::lcos::local::condition_variable> _rwq;

};

}

typedef cor::Mailbox::Send1_action_Mailbox Send1_action_Mailbox;
typedef cor::Mailbox::Send2_action_Mailbox Send2_action_Mailbox;
typedef cor::Mailbox::Receive1_action_Mailbox Receive1_action_Mailbox;
typedef cor::Mailbox::Receive2_action_Mailbox Receive2_action_Mailbox;
typedef cor::Mailbox::Receive_to_map_action_Mailbox Receive_to_map_action_Mailbox;
typedef cor::Mailbox::Broadcast_action_Mailbox Broadcast_action_Mailbox;
typedef cor::Mailbox::Send3_action_Mailbox Send3_action_Mailbox;
typedef cor::Mailbox::Receive3_action_Mailbox Receive3_action_Mailbox;
typedef cor::Mailbox::SendNewClos_action_Mailbox SendNewClos_action_Mailbox;
typedef cor::Mailbox::ReceiveNewClos_action_Mailbox ReceiveNewClos_action_Mailbox;

HPX_REGISTER_ACTION_DECLARATION(Send1_action_Mailbox);
HPX_REGISTER_ACTION_DECLARATION(Send2_action_Mailbox);
HPX_REGISTER_ACTION_DECLARATION(Receive1_action_Mailbox);
HPX_REGISTER_ACTION_DECLARATION(Receive2_action_Mailbox);
HPX_REGISTER_ACTION_DECLARATION(Receive_to_map_action_Mailbox);
HPX_REGISTER_ACTION_DECLARATION(Broadcast_action_Mailbox);
HPX_REGISTER_ACTION_DECLARATION(Send3_action_Mailbox);
HPX_REGISTER_ACTION_DECLARATION(Receive3_action_Mailbox);
HPX_REGISTER_ACTION_DECLARATION(SendNewClos_action_Mailbox);
HPX_REGISTER_ACTION_DECLARATION(ReceiveNewClos_action_Mailbox);


#endif
