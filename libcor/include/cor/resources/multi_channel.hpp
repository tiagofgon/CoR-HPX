#ifndef COR_MULTICHANNEL_HPP
#define COR_MULTICHANNEL_HPP

#include "cor/resources/resource.hpp"
#include "cor/elements/smulti_channel.hpp"

#include <hpx/hpx.hpp>


namespace cor {

template <typename T>
struct MultiChannel: public Resource, public hpx::components::component_base<MultiChannel<T>>
{

typedef typename hpx::components::component_base<MultiChannel<T>>::wrapping_type wrapping_type;
typedef MultiChannel type_holder;
typedef Resource base_type_holder;

protected:
    template <typename ... Args>
    explicit MultiChannel(idp_t idp, std::string const& myself, Args ... args);

public:
    MultiChannel();
    ~MultiChannel();

    hpx::future<T> Get(std::string const& partner, std::size_t step);
    void Set(T&& t, std::string const& partner, std::size_t step);

    HPX_DEFINE_COMPONENT_ACTION(MultiChannel, Get, Get_action_MultiChannel);
    HPX_DEFINE_COMPONENT_ACTION(MultiChannel, Set, Set_action_MultiChannel);


private:
    SMultiChannel<T> _smultichannel;
};


}


#include "cor/resources/multi_channel.tpp"

#define REGISTER_MULTICHANNEL_DECLARATION(type)                  \
    HPX_REGISTER_ACTION_DECLARATION(                      \
        cor::MultiChannel<type>::Get_action_MultiChannel,                    \
        HPX_PP_CAT(__MultiChannel_Get_action_MultiChannel_, type));     \
    HPX_REGISTER_ACTION_DECLARATION(                      \
        cor::MultiChannel<type>::Set_action_MultiChannel,                   \
        HPX_PP_CAT(__MultiChannel_Set_action_MultiChannel_, type));
/**/

#define REGISTER_MULTICHANNEL(type)                              \
    HPX_REGISTER_ACTION(                                  \
        cor::MultiChannel<type>::Get_action_MultiChannel,                    \
        HPX_PP_CAT(__MultiChannel_Get_action_MultiChannel_, type));     \
    HPX_REGISTER_ACTION(                                  \
        cor::MultiChannel<type>::Set_action_MultiChannel,                   \
        HPX_PP_CAT(__MultiChannel_Set_action_MultiChannel_, type));    \
    typedef ::hpx::components::component< cor::MultiChannel<type> > HPX_PP_CAT(__MultiChannel_, type); \
    HPX_REGISTER_COMPONENT(HPX_PP_CAT(__MultiChannel_, type))
/**/

#endif
