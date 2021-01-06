#include "cor/elements/mailbox.hpp"

#include "cor/system/system.hpp"
#include "cor/system/pod_client.hpp"


namespace cor {

Mailbox::Mailbox() = default;
Mailbox::~Mailbox() = default;

Mailbox::Mailbox(idp_t idp) : 
    _idp{idp},
    _channel{hpx::find_here()},
    _channel_ctx{hpx::find_here()}
{
    //std::cout << "Criado um objeto da classe \"Mailbox\", com idp: " << _idp << std::endl;

    std::string channel_name = "Mailbox-" + std::to_string(_idp);
    _channel.register_as(channel_name);

    std::string channel_ctx_name = "Mailbox_ctx-" + std::to_string(_idp);
    _channel_ctx.register_as(channel_ctx_name);


}

// Mailbox::Mailbox(Mailbox&&) noexcept = default;

// Mailbox& Mailbox::operator=(Mailbox&&) noexcept = default;

void Mailbox::Send1(idp_t dest, Message const& msg_original) const
{
    Message msg(msg_original);
    msg.SetSender(_idp);

    std::vector<char> buffer;
    hpx::serialization::output_archive oarchive(buffer);
    oarchive << msg;

    std::string channel_name = "Mailbox-" + std::to_string(dest);
    hpx::lcos::channel<data_type> send_channel;
    send_channel.connect_to(channel_name);
    send_channel.set(buffer);
}

void Mailbox::Send2(std::vector<idp_t> const& dests, Message const& msg) const
{
    for (auto dest: dests)
        Send1(dest, msg);
}

Message Mailbox::Receive1()
{
    if(_messages_order.empty()) {
        data_type m = _channel.get(hpx::launch::sync);

        Message msg;
        hpx::serialization::input_archive iarchive(m);

        iarchive >> msg;
        return msg;
    }
    else {
        auto source = _messages_order[0];
        Message msg = _mailboxes[source].front(); // get first message

        _mailboxes[source].pop(); // delete the message in the queue
        _messages_order.erase(_messages_order.begin()); // delete the first message

        return msg;
    }
    
}

Message Mailbox::Receive2(idp_t source)
{
    // if there are no saved source messages
    if(_mailboxes[source].empty()) {
        data_type m = _channel.get(hpx::launch::sync);
        Message msg;
        hpx::serialization::input_archive iarchive(m);
        iarchive >> msg;
        auto sender_idp = msg.Sender();
        _mailboxes[source].push(msg);
        _messages_order.push_back(sender_idp);

        while(sender_idp != source) {
            data_type m = _channel.get(hpx::launch::sync);
            Message msg;
            hpx::serialization::input_archive iarchive(m);
            iarchive >> msg;
            auto sender_idp = msg.Sender();
            _mailboxes[source].push(msg);
            _messages_order.push_back(sender_idp);
        }
        
        // received a message from the correct source
        Message msg_received = _mailboxes[source].front(); // get the first message
        _mailboxes[source].pop(); // delete the message in the queue
        _messages_order.pop_back(); // delete the last message

        return msg_received;
    }
    else { // if there are already received messages from the source
        Message msg_received = _mailboxes[source].front(); // get the first message
        _mailboxes[source].pop(); // delete the message in the queue
        
        // delete the messages in the general queue of received messages
        for(int i=0; i<_messages_order.size(); i++) {
            if(_messages_order[i] == source) {
                _messages_order.erase(_messages_order.begin()+i);
            }
        }

        return msg_received;
    }

}

void Mailbox::Broadcast(idp_t clos, Message const& msg)
{
    auto sorg = global::pod->GetLocalResource<cor::Closure_Client>(clos);
    auto dests = sorg->GetMemberList();
    Send2(dests, msg);
}

void Mailbox::Send3(idm_t rank, idp_t clos, Message const& msg)
{
    auto sorg = global::pod->GetLocalResource<cor::Closure_Client>(clos);
    auto dest = sorg->GetIdp(rank);
    Send1(dest, msg);
}

Message Mailbox::Receive3(idm_t rank, idp_t clos)
{
    auto sorg = global::pod->GetLocalResource<cor::Closure_Client>(clos);
    auto source = sorg->GetIdp(rank);
    return Receive2(source);
}

void Mailbox::SendNewClos(Message const& msg)
{
    std::vector<char> buffer;
    hpx::serialization::output_archive oarchive(buffer);
    oarchive << msg;
    std::cout << "SendNewClos" << std::endl;
    _channel_ctx.set(buffer);
    std::cout << "SendNewClos" << std::endl;
}

idp_t Mailbox::ReceiveNewClos(std::string const& ctx)
{
    // If the ctx context has not been created yet
    if(_mailboxes_ctr.find(ctx) == _mailboxes_ctr.end()) {
        data_type m = _channel_ctx.get(hpx::launch::sync);
        Message msg;
        hpx::serialization::input_archive iarchive(m);
        iarchive >> msg;
        auto clos_idp = msg.Sender();
        std::string context = msg.Get<std::string>();
        _mailboxes_ctr[context] = clos_idp;

        while(context != ctx) {
            data_type m = _channel_ctx.get(hpx::launch::sync);
            Message msg;
            hpx::serialization::input_archive iarchive(m);
            iarchive >> msg;
            auto clos_idp = msg.Sender();
            std::string context = msg.Get<std::string>();
            _mailboxes_ctr[context] = clos_idp;
        }
        return _mailboxes_ctr[ctx];
    }
    else {
        return _mailboxes_ctr[ctx];
    }


}


}


typedef cor::Mailbox Mailbox;
typedef hpx::components::component<Mailbox> Mailbox_type;

HPX_REGISTER_COMPONENT(Mailbox_type, Mailbox);


typedef cor::Mailbox::Send1_action_Mailbox Send1_action_Mailbox;
typedef cor::Mailbox::Send2_action_Mailbox Send2_action_Mailbox;
typedef cor::Mailbox::Receive1_action_Mailbox Receive1_action_Mailbox;
typedef cor::Mailbox::Receive2_action_Mailbox Receive2_action_Mailbox;
typedef cor::Mailbox::Broadcast_action_Mailbox Broadcast_action_Mailbox;
typedef cor::Mailbox::Send3_action_Mailbox Send3_action_Mailbox;
typedef cor::Mailbox::Receive3_action_Mailbox Receive3_action_Mailbox;
typedef cor::Mailbox::SendNewClos_action_Mailbox SendNewClos_action_Mailbox;
typedef cor::Mailbox::ReceiveNewClos_action_Mailbox ReceiveNewClos_action_Mailbox;

HPX_REGISTER_ACTION(Send1_action_Mailbox);
HPX_REGISTER_ACTION(Send2_action_Mailbox);
HPX_REGISTER_ACTION(Receive1_action_Mailbox);
HPX_REGISTER_ACTION(Receive2_action_Mailbox);
HPX_REGISTER_ACTION(Broadcast_action_Mailbox);
HPX_REGISTER_ACTION(Send3_action_Mailbox);
HPX_REGISTER_ACTION(Receive3_action_Mailbox);
HPX_REGISTER_ACTION(SendNewClos_action_Mailbox);
HPX_REGISTER_ACTION(ReceiveNewClos_action_Mailbox);