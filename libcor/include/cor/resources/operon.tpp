#ifdef COR_OPERON_HPP


namespace cor {

template <typename R, typename ... P>
Operon<R(P...)>::~Operon() = default;

template <typename R, typename ... P>
Operon<R(P...)>::Operon(idp_t idp, std::function<R(P...)> const& f) :
    Resource{idp},
    _executor_pool{idp, f}
{
    std::cout << "Criado um componente \"Operon\", com idp: " << idp << std::endl;
}


/* Executor's interface */
template <typename R, typename ... P>
template <typename ... Args>
void Operon<R(P...)>::Run(Args&&... args)
{
    // return _executor.Run(std::forward<Args>(args)... );
}

template <typename R, typename ... P>
void Operon<R(P...)>::Wait()
{
    // return _executor.Wait();
}

// template <typename R, typename ... P>
// R Operon<R(P...)>::Get()
// {
//     // return _executor.Get();
//     R aux;
//     return aux;
// }

template <typename R, typename ... P>
void Operon<R(P...)>::ChangeIdp(idp_t idp)
{
    // return _executor.ChangeIdp(idp);
}

template <typename R, typename ... P>
void Operon<R(P...)>::ResumeIdp()
{
    // return _executor.ResumeIdp();
}

template <typename R, typename ... P>
idp_t Operon<R(P...)>::CurrentIdp()
{
    // return _executor.CurrentIdp();
    return 111;
}

template <typename R, typename ... P>
idp_t Operon<R(P...)>::OriginalIdp()
{
    // return _executor.OriginalIdp();
    return 111;
}

template <typename R, typename ... P>
idp_t Operon<R(P...)>::GetExecutorIdp()
{
    // return _executor.GetExecutorIdp();
    return 111;
}

template <typename R, typename ... P>
void Operon<R(P...)>::Dispatch(hpx::function<void(std::shared_ptr<void>)> const& f)
{
    return _executor_pool.Dispatch(f);
}




}


#endif
