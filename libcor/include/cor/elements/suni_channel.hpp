#ifndef COR_SUNICHANNEL_HPP
#define COR_SUNICHANNEL_HPP

#include <hpx/include/lcos.hpp>

#include <array>

template <typename T>
struct SUniChannel
{

public:
    typedef hpx::lcos::channel<T> channel_type;

    SUniChannel(idp_t idp) :
        _idp{idp},
        _channel{hpx::find_here()}
    {

    }


    void Set(T&& t, std::size_t step)
    {
        _channel.set(hpx::launch::apply, std::move(t), step);
    }

    hpx::future<T> Get(std::size_t step)
    {
        return _channel.get(hpx::launch::async, step);
    }


private:
    idp_t _idp;
    hpx::lcos::channel<T> _channel;


};

#endif