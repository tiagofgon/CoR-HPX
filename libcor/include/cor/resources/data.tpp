#ifdef COR_DATA_HPP

#include "cor/system/system.hpp"
#include "cor/system/pod_client.hpp"

namespace cor {

template <typename T>
Data<T>::Data() = default;

template <typename T>
template <typename ... Args>
Data<T>::Data(idp_t idp, Args&&... args) : 
    base_type(idp),
    _value{idp, std::forward<Args>(args)...}
{
    std::cout << "Criado um componente \"Data\", com idp: " << idp << std::endl;
}

template <typename T>
Data<T>::~Data() = default;

// template <typename T>
// Data<T>::Data(Data<T>&&) noexcept = default;

// template <typename T>
// Data<T>& Data<T>::operator=(Data<T>&&) noexcept = default;

template <typename T>
T Data<T>::Fetch()
{
    return _value.Fetch();
}

template <typename T>
int Data<T>::Acquire()
{
    return _value.Acquire();
}

template <typename T>
void Data<T>::Release()
{
    return _value.Release();
}


}

#endif
