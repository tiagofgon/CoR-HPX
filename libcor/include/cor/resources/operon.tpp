#ifdef COR_OPERON_HPP


namespace cor {


template < typename ... Args >
void Operon::Dispatch(hpx::function<void(Args...)> func, Args ... args) {
    return _executor_pool.Dispatch(func, args...);
}

template < typename Func, typename ... Args >
void Operon::Dispatch(Func&& func, Args&&... args) {
    return _executor_pool.Dispatch(std::forward<Func>(func), std::forward<Args>(args)...);
}

template < typename Func>
void Operon::Dispatch(Func&& func) {
    return _executor_pool.Dispatch(std::forward<Func>(func));
}



}


#endif
