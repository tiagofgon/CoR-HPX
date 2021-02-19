#ifndef COR_COMMUNICATOR_HPP
#define COR_COMMUNICATOR_HPP

#include <hpx/include/lcos.hpp>

#include <array>

template <typename T>
struct Communicator
{
    enum neighbor {
        up = 0,
        down = 1,
    };

    typedef hpx::lcos::channel<T> channel_type;

    // rank: our rank in the system
    // num: number of participating partners
    Communicator(idp_t idp, std::string myself, std::string partner)
    {
        // Retrieve the channel from our upper neighbor from which we receive
        // the row we need to update the first row in our partition.
        recv[0] = hpx::find_from_basename<channel_type>(partner, 0);

        // Create the channel we use to send our first row to our upper
        // neighbor
        send[0] = channel_type(hpx::find_here());
        // Register the channel with a name such that our neighbor can find it.
        hpx::register_with_basename(myself, send[0], 0);

    }


    void Set(T&& t, std::size_t step)
    {
        // Send our data to the neighbor n using fire and forget semantics
        // Synchronization happens when receiving values.
        send[0].set(hpx::launch::apply, std::move(t), step);
    }

    hpx::future<T> Get(std::size_t step)
    {
        // Get our data from our neighbor, we return a future to allow the
        // algorithm to synchronize.
        return recv[0].get(hpx::launch::async, step);
    }

    std::array<hpx::lcos::channel<T>, 1> recv;
    std::array<hpx::lcos::channel<T>, 1> send;
};

#endif