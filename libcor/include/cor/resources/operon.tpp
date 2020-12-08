#ifdef COR_OPERON_HPP


namespace cor {


template < typename ... Args >
void Operon::Dispatch(hpx::function<void(Args...)> func, Args ... args) {
    return _executor_pool.Dispatch(func, args...);
}

}


#endif
