#ifdef COR_VALUE_HPP

// #include "cor/system/system.hpp"
// #include "cor/system/pod_client.hpp"

namespace cor {

template <typename T>
Value<T>::Value() = default;

template <typename T>
template <typename ... Args>
Value<T>::Value(idp_t idp, Args&&... args) :
    _idp{idp},
    _value{std::forward<Args>(args)...}
{
    std::cout << "Criado um objeto da classe \"Value\", com idp: " << _idp << std::endl;
}

template <typename T>
Value<T>::~Value() = default;

// template <typename T>
// Value<T>::Value(Value<T>&&) noexcept = default;

// template <typename T>
// Value<T>& Value<T>::operator=(Value<T>&&) noexcept = default;

// template <typename T>
// T* Value<T>::Get()
// {
//     return _value.get();
// }

template <typename T>
T Value<T>::Fetch() const
{
    return _value;
}

template <typename T>
int Value<T>::Acquire()
{
    // while(atomvar != 0) {

    // }

    // return atomvar.fetch_add(1, std::memory_order_relaxed);
    return 1;
}

template <typename T>
void Value<T>::Release()
{
    // atomvar--;
    return;
}

}

#endif
