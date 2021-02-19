#ifndef COR_DUPLEXCHANNEL_HPP
#define COR_DUPLEXCHANNEL_HPP

#include "cor/resources/resource.hpp"
#include "cor/elements/communicator.hpp"

#include <hpx/hpx.hpp>


namespace cor {

template <typename T>
struct DuplexChannel: public Resource, public hpx::components::component_base<DuplexChannel<T>>
{

typedef typename hpx::components::component_base<DuplexChannel<T>>::wrapping_type wrapping_type;
typedef DuplexChannel type_holder;
typedef Resource base_type_holder;

protected:
    explicit DuplexChannel(idp_t idp, std::string myself, std::string partner);

public:
    DuplexChannel();
    ~DuplexChannel();

    hpx::future<T> Get(std::size_t step);
    void Set(T&& t, std::size_t step);

    HPX_DEFINE_COMPONENT_ACTION(DuplexChannel, Get, Get_action_DuplexChannel);
    HPX_DEFINE_COMPONENT_ACTION(DuplexChannel, Set, Set_action_DuplexChannel);


private:
    Communicator<T> _communicator;
};


}


#include "cor/resources/duplex_channel.tpp"

#define REGISTER_DUPLEXCHANNEL_DECLARATION(type)                  \
    HPX_REGISTER_ACTION_DECLARATION(                      \
        cor::DuplexChannel<type>::Get_action_DuplexChannel,                    \
        HPX_PP_CAT(__DuplexChannel_Get_action_DuplexChannel_, type));     \
    HPX_REGISTER_ACTION_DECLARATION(                      \
        cor::DuplexChannel<type>::Set_action_DuplexChannel,                   \
        HPX_PP_CAT(__DuplexChannel_Set_action_DuplexChannel_, type));
/**/

#define REGISTER_DUPLEXCHANNEL(type)                              \
    HPX_REGISTER_ACTION(                                  \
        cor::DuplexChannel<type>::Get_action_DuplexChannel,                    \
        HPX_PP_CAT(__DuplexChannel_Get_action_DuplexChannel_, type));     \
    HPX_REGISTER_ACTION(                                  \
        cor::DuplexChannel<type>::Set_action_DuplexChannel,                   \
        HPX_PP_CAT(__DuplexChannel_Set_action_DuplexChannel_, type));    \
    typedef ::hpx::components::component< cor::DuplexChannel<type> > HPX_PP_CAT(__DuplexChannel_, type); \
    HPX_REGISTER_COMPONENT(HPX_PP_CAT(__DuplexChannel_, type))
/**/

#endif
