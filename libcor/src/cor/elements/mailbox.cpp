

#include <hpx/hpx.hpp>

#include "cor/elements/mailbox.hpp"

#include "cor/system/system.hpp"
#include "cor/system/pod_client.hpp"

#include <queue>
#include <atomic>
#include <functional>
#include <mutex>

namespace cor {

Mailbox::Mailbox() = default;

Mailbox::Mailbox(idp_t idp) : 
    _idp{idp},
    _channel{hpx::find_here()}
{
    std::cout << "Criado um objeto da classe \"Mailbox\", com idp: " << _idp << std::endl;
    std::string channel_name = "Mailbox-" + std::to_string(_idp);
    _channel.register_as(channel_name);
}

Mailbox::~Mailbox() = default;

// Mailbox::Mailbox(Mailbox&&) noexcept = default;

// Mailbox& Mailbox::operator=(Mailbox&&) noexcept = default;

// void Mailbox::Send1(idp_t dest, Message const& msg_original) const
// {
//     Message msg(msg_original);
//     // msg = msg_original;
//     msg.SetSender(_idp);

//     std::vector<char> buffer;
//     hpx::serialization::output_archive oarchive(buffer);
//     oarchive << msg;
    

//     // std::string channel_name = "Mailbox-" + std::to_string(dest);
//     // hpx::lcos::channel<data_type> send_channel;
//     // send_channel.connect_to(channel_name);
//     // send_channel.set(buffer);
//     std::cout << "Send1-1" << std::endl;
//     // Enviar mensagens para a mailbox do agente recetor
//     hpx::id_type dest_gid = global::pod->GetAgentMailbox(dest);
//     std::cout << "Send1-2" << std::endl;
//     typedef cor::Mailbox::Receive_to_map_action_Mailbox action_type;
//     hpx::async<action_type>(dest_gid, _idp, msg).get();
//     std::cout << "Send1-3" << std::endl;

// }

// send versão channel
void Mailbox::Send1(idp_t dest, Message const& msg_original) const
{
    Message msg(msg_original);
    // msg = msg_original;
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

// Message Mailbox::Receive1() const
// {
//     std::cout << "MENSAGEM a receber" << '\n';
//     data_type m = _channel.get(hpx::launch::sync);
//     std::cout << "MENSAGEM recebida" << '\n';

//     Message msg;
//     hpx::serialization::input_archive iarchive(m);

//     iarchive >> msg;

//     return msg;

    
// }

// Received with channels, without source
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
        Message msg = _mailboxes[source].front(); // buscar a primeira mensagem
        _mailboxes[source].pop(); // eliminar a mensagem na fila de espera
        _messages_order.erase(_messages_order.begin()); // eliminar a primeira mensagem

        return msg;
    }



    

    
}

// Message Mailbox::Receive2(idp_t source)
// {
// _mtx.lock();
// std::cout << "Receive2-1" << std::endl;
//     if(_mailboxes[source].empty() && future_constructed[source] == false) {
//         hpx::lcos::local::promise<void> pr;
//         _promises[source] = std::move(pr);
//         _futures[source] = _promises[source].get_future();
        
//         future_constructed[source] = true;
//         std::cout << "end-1" << std::endl;
//         _mtx.unlock();
//         std::cout << "fiz get no idp: " << source << std::endl;
//         _futures[source].get(); // fica à espera que enviem a mensagem
//         _mtx.lock();
//         std::cout << "Receive2-2" << std::endl;
//         Message msg = _mailboxes[source].front(); // buscar a primeira mensagem
//         _mailboxes[source].pop(); // eliminar a mensagem na fila de espera
//         future_constructed[source] = false;
//         std::cout << "end-2" << std::endl;
//         _mtx.unlock();

//         return msg;
//     }
//     else if(_mailboxes[source].empty() && future_constructed[source] == true){
//         std::cout << "end-3" << std::endl;
//         _mtx.unlock();
//         std::cout << "fiz get no idp: " << source << std::endl;
//         _futures[source].get(); // fica à espera que enviem a mensagem
//         _mtx.lock();
//         std::cout << "Receive2-3" << std::endl;
//         Message msg = _mailboxes[source].front(); // buscar a primeira mensagem
//         _mailboxes[source].pop(); // eliminar a mensagem na fila de espera
//         future_constructed[source] = false;
//         std::cout << "end-4" << std::endl;
//         _mtx.unlock();

//         return msg;
//     }

//     else {
//         Message msg = _mailboxes[source].front(); // buscar a primeira mensagem
//         _mailboxes[source].pop(); // eliminar a mensagem na fila de espera
//         future_constructed[source] = false;
//         std::cout << "end-5" << std::endl;
//         _mtx.unlock();

//         return msg;        
//     }


// }

// Receive versão channel
Message Mailbox::Receive2(idp_t source)
{
    // se não há mensagens do source guardadas
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
        
        // recebeu uma mensagem do source suposto
        Message msg_received = _mailboxes[source].front(); // buscar a primeira mensagem
        _mailboxes[source].pop(); // eliminar a mensagem na fila de espera
        _messages_order.pop_back(); // eliminar a ultima mensagem

        return msg_received;
    }
    else { // Se já tiver mensagens do source recebidas
        Message msg_received = _mailboxes[source].front(); // buscar a primeira mensagem
        _mailboxes[source].pop(); // eliminar a mensagem na fila de espera
        
        // apagar a mensagens na fila geral de mensagens recebidas
        for(int i=0; i<_messages_order.size(); i++) {
            if(_messages_order[i] == source) {
                _messages_order.erase(_messages_order.begin()+i);
            }
        }

        return msg_received;
    }

}

void Mailbox::Receive_to_map(idp_t source, Message const& msg)
{
    // _mtx.lock();
    // std::cout << "Receive_to_map" << std::endl;
    // if(_mailboxes[source].empty() && future_constructed[source] == false) {
    //     hpx::lcos::local::promise<void> pr;
    //     _promises[source] = std::move(pr);
    //     _futures[source] = _promises[source].get_future();

    //     _promises[source].set_value();
    //     future_constructed[source] = true;
    // }
    // else if(_mailboxes[source].empty() && future_constructed[source] == true){
    //     _promises[source].set_value();
    // }
    // std::cout << "fiz set no idp: " << source << std::endl;
    // _mailboxes[source].push(msg);
    // std::cout << "end" << std::endl;
    // _mtx.unlock();
}


void Mailbox::Broadcast(idp_t clos, Message const& msg)
{
    auto sorg = global::pod->GetLocalResource<cor::Closure_Client>(clos);
    auto dests = sorg->GetMemberList();
    Send2(dests, msg);
}

void Mailbox::Send3(idm_t rank, idp_t clos, Message const& msg)
{
    std::cout << "Send3-1" << std::endl;
    auto sorg = global::pod->GetLocalResource<cor::Closure_Client>(clos);
    std::cout << "Send3-2" << std::endl;
    auto dest = sorg->GetIdp(rank);
    std::cout << "Send3-3" << std::endl;
    Send1(dest, msg);
    std::cout << "Send3-4" << std::endl;
}

Message Mailbox::Receive3(idm_t rank, idp_t clos)
{
    auto sorg = global::pod->GetLocalResource<cor::Closure_Client>(clos);
    auto source = sorg->GetIdp(rank);
    return Receive2(source);
}

}

typedef cor::Mailbox Mailbox;
typedef hpx::components::component<Mailbox> Mailbox_type;

HPX_REGISTER_COMPONENT(Mailbox_type, Mailbox);


typedef cor::Mailbox::Send1_action_Mailbox Send1_action_Mailbox;
typedef cor::Mailbox::Send2_action_Mailbox Send2_action_Mailbox;
typedef cor::Mailbox::Receive1_action_Mailbox Receive1_action_Mailbox;
typedef cor::Mailbox::Receive2_action_Mailbox Receive2_action_Mailbox;
typedef cor::Mailbox::Receive_to_map_action_Mailbox Receive_to_map_action_Mailbox;
typedef cor::Mailbox::Broadcast_action_Mailbox Broadcast_action_Mailbox;
typedef cor::Mailbox::Send3_action_Mailbox Send3_action_Mailbox;
typedef cor::Mailbox::Receive3_action_Mailbox Receive3_action_Mailbox;

HPX_REGISTER_ACTION(Send1_action_Mailbox);
HPX_REGISTER_ACTION(Send2_action_Mailbox);
HPX_REGISTER_ACTION(Receive1_action_Mailbox);
HPX_REGISTER_ACTION(Receive2_action_Mailbox);
HPX_REGISTER_ACTION(Receive_to_map_action_Mailbox);
HPX_REGISTER_ACTION(Broadcast_action_Mailbox);
HPX_REGISTER_ACTION(Send3_action_Mailbox);
HPX_REGISTER_ACTION(Receive3_action_Mailbox);