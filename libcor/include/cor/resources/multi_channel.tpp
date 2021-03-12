#ifdef COR_MULTICHANNEL_HPP

namespace cor {

template <typename T>
MultiChannel<T>::MultiChannel() = default;

template <typename T>
MultiChannel<T>::~MultiChannel() = default;

template <typename T>
template <typename ... Args>
MultiChannel<T>::MultiChannel(idp_t idp, std::string const& myself, Args ... args) :
    Resource{idp},
    _smultichannel{idp, myself, args...}
{
    //std::cout << "Criado um componente \"MultiChannel\", com idp: " << idp << std::endl;
}

template <typename T>
hpx::future<T> MultiChannel<T>::Get(std::string const& partner, std::size_t step) {
    return _smultichannel.Get(partner, step);
}

template <typename T>
void MultiChannel<T>::Set(T&& t, std::string const& partner, std::size_t step) {
    return _smultichannel.Set(std::move(t), partner, step);
}


}


#endif
