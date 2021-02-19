#ifndef COR_SWITCH_HPP
#define COR_SWITCH_HPP

#include "cor/resources/resource.hpp"
#include "cor/elements/switch_hpx.hpp"

#include <hpx/hpx.hpp>


namespace cor {

template <typename T>
struct Switch: public Resource, public hpx::components::component_base<Switch<T>>
{

typedef typename hpx::components::component_base<Switch<T>>::wrapping_type wrapping_type;
typedef Switch type_holder;
typedef Resource base_type_holder;

protected:
    template <typename ... Args>
    explicit Switch(idp_t idp, std::string const& myself, Args ... args);

public:
    Switch();
    ~Switch();

    hpx::future<T> Get(std::string const& partner, std::size_t step);
    void Set(T&& t, std::string const& partner, std::size_t step);

    HPX_DEFINE_COMPONENT_ACTION(Switch, Get, Get_action_Switch);
    HPX_DEFINE_COMPONENT_ACTION(Switch, Set, Set_action_Switch);


private:
    SwitchHpx<T> _switchhpx;
};


}


#include "cor/resources/switch.tpp"

#define REGISTER_SWITCH_DECLARATION(type)                  \
    HPX_REGISTER_ACTION_DECLARATION(                      \
        cor::Switch<type>::Get_action_Switch,                    \
        HPX_PP_CAT(__Switch_Get_action_Switch_, type));     \
    HPX_REGISTER_ACTION_DECLARATION(                      \
        cor::Switch<type>::Set_action_Switch,                   \
        HPX_PP_CAT(__Switch_Set_action_Switch_, type));
/**/

#define REGISTER_SWITCH(type)                              \
    HPX_REGISTER_ACTION(                                  \
        cor::Switch<type>::Get_action_Switch,                    \
        HPX_PP_CAT(__Switch_Get_action_Switch_, type));     \
    HPX_REGISTER_ACTION(                                  \
        cor::Switch<type>::Set_action_Switch,                   \
        HPX_PP_CAT(__Switch_Set_action_Switch_, type));    \
    typedef ::hpx::components::component< cor::Switch<type> > HPX_PP_CAT(__Switch_, type); \
    HPX_REGISTER_COMPONENT(HPX_PP_CAT(__Switch_, type))
/**/

#endif
