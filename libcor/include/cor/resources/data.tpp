#ifdef COR_DATA_HPP

#include "cor/system/system.hpp"
#include "cor/system/pod_client.hpp"

namespace cor {

template <typename T>
Data<T>::Data() = default;

template <typename T>
Data<T>::~Data() = default;

template <typename T>
template <typename ... Args>
Data<T>::Data(idp_t idp, Args&&... args) : 
    base_type(idp),
    _value{idp, std::forward<Args>(args)...}
{
    std::cout << "Criado um componente \"Data\", com idp: " << idp << std::endl;
}

template <typename T>
T Data<T>::Fetch()
{
    return _value.Fetch();
}

template <typename T>
T* Data<T>::Get()
{
    return _value.Get();
}

template <typename T>
template <typename F, typename ... Args>
auto Data<T>::Run(hpx::function<F> func, Args... args)
{
    T* value = Get();
    return func(*value, args...);
}


}


#endif
