#ifndef COR_DATA_HPP
#define COR_DATA_HPP

#include "cor/resources/resource_migrable.hpp"
#include "cor/elements/value.hpp"

#include <hpx/hpx.hpp>


namespace cor {

template <typename T>
struct Data: public hpx::components::abstract_migration_support< hpx::components::component_base<Data<T>>, ResourceMigrable >
{

using base_type = hpx::components::abstract_migration_support<
    hpx::components::component_base<Data<T>>, ResourceMigrable >;

typedef typename hpx::components::component_base<Data<T>>::wrapping_type wrapping_type;
typedef Data type_holder;
typedef ResourceMigrable base_type_holder;

friend class hpx::serialization::access;

protected:
    template <typename ... Args>
    explicit Data(idp_t idp, Args&&... args);

public:
    Data();
    ~Data();

    // Components which should be migrated using hpx::migrate<> need to
    // be Serializable and CopyConstructable. Components can be
    // MoveConstructable in which case the serialized data is moved into the
    // component's constructor.
    Data(Data&& rsc) :
        base_type(std::move(rsc)),
        _value(std::move(rsc._value))
    {}

    Data& operator=(Data&& rsc)
    {
        this->ResourceMigrable::operator=(std::move(static_cast<ResourceMigrable&>(rsc)));
        _value = rsc._value;
        return *this;
    }

    T &operator*() 
    { 
        std::cout << "AQUI_1" << std::endl;
        return *_value; 
    }

    T const &operator*() const 
    { 
        std::cout << "AQUI_2" << std::endl;
        return _value; 
    }

    T const *operator->() const 
    { 
        std::cout << "AQUI_3" << std::endl;
        return &_value; 
    }

    T *operator->() 
    { 
        std::cout << "AQUI_4" << std::endl;
        return &_value; 
    }

    T Fetch();
    T* Get();

    template <typename F, typename ... Args>
    auto Run(hpx::function<F> func, Args... args);

    HPX_DEFINE_COMPONENT_ACTION(Data, Fetch, Fetch_action_Data);

    template <typename F, typename ... Args>
    struct Run_action_Data
    : hpx::actions::make_action<
        decltype(&Data::Run<F, Args...>),
        &Data::Run<F, Args...>
    >::type
    {};

    template <typename Archive>
    void serialize(Archive& ar, unsigned version)
    {
        ar & hpx::serialization::base_object<ResourceMigrable>(*this);
        ar & _value;
    }
    

private:
    Value<T> _value;
};


}


#include "cor/resources/data.tpp"

#define REGISTER_DATA_DECLARATION(type)                                                                             \
    HPX_REGISTER_ACTION_DECLARATION(                                                                                \
        cor::Data<type>::Fetch_action_Data,                                                                         \
        HPX_PP_CAT(__Data_Fetch_action_Data_, type));                                                               \

#define REGISTER_DATA(type)                                                                                         \
    HPX_REGISTER_ACTION(                                                                                            \
        cor::Data<type>::Fetch_action_Data,                                                                         \
        HPX_PP_CAT(__Data_Fetch_action_Data_, type));                                                               \
    typedef ::hpx::components::component<cor::Data<type>>                                                           \
        HPX_PP_CAT(__Data_type, type);                                                                              \
    typedef cor::Data<type>                                                                                         \
        HPX_PP_CAT(__Data_, type);                                                                                  \
    HPX_REGISTER_DERIVED_COMPONENT_FACTORY(HPX_PP_CAT(__Data_type, type), HPX_PP_CAT(__Data_, type), "ResourceMigrable")    \


#endif
