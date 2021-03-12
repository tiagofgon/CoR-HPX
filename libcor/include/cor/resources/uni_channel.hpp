#ifndef COR_UNICHANNEL_HPP
#define COR_UNICHANNEL_HPP

#include "cor/resources/resource.hpp"
#include "cor/elements/suni_channel.hpp"

#include <hpx/hpx.hpp>


namespace cor {

template <typename T>
struct UniChannel: public Resource, public hpx::components::component_base<UniChannel<T>>
{

typedef typename hpx::components::component_base<UniChannel<T>>::wrapping_type wrapping_type;
typedef UniChannel type_holder;
typedef Resource base_type_holder;

protected:
    explicit UniChannel(idp_t idp);

public:
    UniChannel();
    ~UniChannel();

    hpx::future<T> Get(std::size_t step);
    void Set(T&& t, std::size_t step);

    HPX_DEFINE_COMPONENT_ACTION(UniChannel, Get, Get_action_UniChannel);
    HPX_DEFINE_COMPONENT_ACTION(UniChannel, Set, Set_action_UniChannel);


private:
    SUniChannel<T> _sunichannel;
};


}


#include "cor/resources/uni_channel.tpp"

#define REGISTER_UNICHANNEL_DECLARATION(type)                  \
    HPX_REGISTER_ACTION_DECLARATION(                      \
        cor::UniChannel<type>::Get_action_UniChannel,                    \
        HPX_PP_CAT(__UniChannel_Get_action_UniChannel_, type));     \
    HPX_REGISTER_ACTION_DECLARATION(                      \
        cor::UniChannel<type>::Set_action_UniChannel,                   \
        HPX_PP_CAT(__UniChannel_Set_action_UniChannel_, type));
/**/

#define REGISTER_UNICHANNEL(type)                              \
    HPX_REGISTER_ACTION(                                  \
        cor::UniChannel<type>::Get_action_UniChannel,                    \
        HPX_PP_CAT(__UniChannel_Get_action_UniChannel_, type));     \
    HPX_REGISTER_ACTION(                                  \
        cor::UniChannel<type>::Set_action_UniChannel,                   \
        HPX_PP_CAT(__UniChannel_Set_action_UniChannel_, type));    \
    typedef ::hpx::components::component< cor::UniChannel<type> > HPX_PP_CAT(__UniChannel_, type); \
    HPX_REGISTER_COMPONENT(HPX_PP_CAT(__UniChannel_, type))
/**/

#endif
