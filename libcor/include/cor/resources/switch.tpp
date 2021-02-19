#ifdef COR_SWITCH_HPP

namespace cor {

template <typename T>
Switch<T>::Switch() = default;

template <typename T>
Switch<T>::~Switch() = default;

template <typename T>
template <typename ... Args>
Switch<T>::Switch(idp_t idp, std::string const& myself, Args ... args) :
    Resource{idp},
    _switchhpx{idp, myself, args...}
{
    //std::cout << "Criado um componente \"Switch\", com idp: " << idp << std::endl;
}

template <typename T>
hpx::future<T> Switch<T>::Get(std::string const& partner, std::size_t step) {
    return _switchhpx.Get(partner, step);
}

template <typename T>
void Switch<T>::Set(T&& t, std::string const& partner, std::size_t step) {
    return _switchhpx.Set(std::move(t), partner, step);
}


}


#endif
