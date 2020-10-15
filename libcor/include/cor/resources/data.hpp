#ifndef COR_DATA_HPP
#define COR_DATA_HPP

#include "cor/resources/resource.hpp"
#include "cor/elements/value.hpp"


namespace cor {

template <typename T>
// struct Data: public Resource, public hpx::components::component_base<Data<T>>
struct Data: public hpx::components::abstract_migration_support< hpx::components::component_base<Data<T>>, Resource >
{

using base_type = hpx::components::abstract_migration_support<
    hpx::components::component_base<Data<T>>, Resource >;

typedef typename hpx::components::component_base<Data<T>>::wrapping_type wrapping_type;
typedef Data type_holder;
typedef Resource base_type_holder;

friend class ResourceManager;
friend class hpx::serialization::access;

public:
    Data();
    ~Data();

    template <typename ... Args>
    explicit Data(idp_t idp, Args&&... args);

    // Components which should be migrated using hpx::migrate<> need to
    // be Serializable and CopyConstructable. Components can be
    // MoveConstructable in which case the serialized data is moved into the
    // component's constructor.
    Data(Data&& rsc) :
        base_type(std::move(rsc)),
        _value(std::move(rsc._value))
    {}

    // Data& operator=(Data&& rsc)
    // {
    //     // value = std::move(rsc.value);
    //     return *this;
    // }



    Data& operator=(Data&& rsc)
    {
        this->Resource::operator=(std::move(static_cast<Resource&>(rsc)));
        _value = rsc._value;
        return *this;
    }


    // Data(const Data&) = delete;
    // Data& operator=(const Data&) = delete;

    // Data(Data&&) noexcept;
    // Data& operator=(Data&&) noexcept;

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

	int Acquire();
	void Release();


    HPX_DEFINE_COMPONENT_ACTION(Data, Fetch, Fetch_action_Data);
    HPX_DEFINE_COMPONENT_ACTION(Data, Acquire, Acquire_action_Data);
    HPX_DEFINE_COMPONENT_ACTION(Data, Release, Release_action_Data);



    template <typename Archive>
    void serialize(Archive& ar, unsigned version)
    {
        ar & hpx::serialization::base_object<Resource>(*this);
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
        HPX_PP_CAT(__Data_Fetch_action_Data_, type));                                                                \
    HPX_REGISTER_ACTION_DECLARATION(                                                                                \
        cor::Data<type>::Acquire_action_Data,                                                                         \
        HPX_PP_CAT(__Data_Acquire_action_Data_, type));                                                                \
    HPX_REGISTER_ACTION_DECLARATION(                                                                                \
        cor::Data<type>::Release_action_Data,                                                                         \
        HPX_PP_CAT(__Data_Release_action_Data_, type));                                                                \


#define REGISTER_DATA(type)                                                                                         \
    HPX_REGISTER_ACTION(                                                                                            \
        cor::Data<type>::Fetch_action_Data,                                                                         \
        HPX_PP_CAT(__Data_Fetch_action_Data_, type));                                                               \
    HPX_REGISTER_ACTION(                                                                                            \
        cor::Data<type>::Acquire_action_Data,                                                                         \
        HPX_PP_CAT(__Data_Acquire_action_Data_, type));                                                               \
    HPX_REGISTER_ACTION(                                                                                            \
        cor::Data<type>::Release_action_Data,                                                                         \
        HPX_PP_CAT(__Data_Release_action_Data_, type));                                                               \
    typedef ::hpx::components::component<cor::Data<type>>                                                           \
        HPX_PP_CAT(__Data_type, type);                                                                              \
    typedef cor::Data<type>                                                                                         \
        HPX_PP_CAT(__Data_, type);                                                                                  \
    HPX_REGISTER_DERIVED_COMPONENT_FACTORY(HPX_PP_CAT(__Data_type, type), HPX_PP_CAT(__Data_, type), "Resource")    \


#endif
