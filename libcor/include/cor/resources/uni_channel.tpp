#ifdef COR_UNICHANNEL_HPP

namespace cor {

template <typename T>
UniChannel<T>::UniChannel() = default;

template <typename T>
UniChannel<T>::~UniChannel() = default;

template <typename T>
UniChannel<T>::UniChannel(idp_t idp) :
    Resource{idp},
    _sunichannel{idp}
{
    //std::cout << "Criado um componente \"UniChannel\", com idp: " << idp << std::endl;
}

template <typename T>
hpx::future<T> UniChannel<T>::Get(std::size_t step) {
    return _sunichannel.Get(step);
}

template <typename T>
void UniChannel<T>::Set(T&& t, std::size_t step) {
    return _sunichannel.Set(std::move(t), step);
}


}


#endif
