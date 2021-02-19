#ifdef COR_DUPLEXCHANNEL_HPP

namespace cor {

template <typename T>
DuplexChannel<T>::DuplexChannel() = default;

template <typename T>
DuplexChannel<T>::~DuplexChannel() = default;

template <typename T>
DuplexChannel<T>::DuplexChannel(idp_t idp, std::string myself, std::string partner) :
    Resource{idp},
    _communicator{idp, myself, partner}
{
    //std::cout << "Criado um componente \"DuplexChannel\", com idp: " << idp << std::endl;
}

template <typename T>
hpx::future<T> DuplexChannel<T>::Get(std::size_t step) {
    return _communicator.Get(step);
}

template <typename T>
void DuplexChannel<T>::Set(T&& t, std::size_t step) {
    return _communicator.Set(std::move(t), step);
}


}


#endif
