#ifndef COR_SWITCHHPX_HPP
#define COR_SWITCHHPX_HPP

#include <hpx/include/lcos.hpp>

#include <array>

template <typename T>
struct SwitchHpx
{

public:
    typedef hpx::lcos::channel<T> channel_type;

    template <typename ... Args>
    SwitchHpx(idp_t idp, std::string const& myself, Args ... args) :
        _idp{idp},
        _myself{myself}
    {
        // cria e regista os canais de escrita para todos os meus parceiros, com nome partner + myself,
        // que irao ser os canais de leitura deles das minhas mensagens
        for(const auto partners_name : {args...}) {
            std::string sender_channel = partners_name + myself;
            send[sender_channel] = channel_type(hpx::find_here());
            hpx::register_with_basename(sender_channel, send[sender_channel], 0);
        }


        // procura e atualiza os meus canais de leitura dos meus parceiros, com nome myself + partner,
        // que irao ser os canais de escrita deles para eu ler
        for(const auto partners_name : {args...}) {
            std::string receiver_channel = myself + partners_name;
            recv[receiver_channel] = hpx::find_from_basename<channel_type>(receiver_channel, 0);
        }
    }


    void Set(T&& t, std::string const& partner, std::size_t step)
    {
        // Send our data to the partner using fire and forget semantics
        // Synchronization happens when receiving values.
        std::string sender_channel = partner + _myself;
        send[sender_channel].set(hpx::launch::apply, std::move(t), step);
    }

    hpx::future<T> Get(std::string const& partner, std::size_t step)
    {
        // Get our data from our partner, we return a future to allow the
        // algorithm to synchronize.
        std::string receiver_channel = _myself + partner;
        return recv[receiver_channel].get(hpx::launch::async, step);
    }


private:
    idp_t _idp;
    std::string _myself;

    std::map<std::string, hpx::lcos::channel<T>> recv;
    std::map<std::string, hpx::lcos::channel<T>> send;


};

#endif