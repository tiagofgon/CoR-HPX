#ifdef EXECUTOR_POOL_HPP

namespace cor
{

template < typename ... Args >
void ExecutorPool::Dispatch(hpx::function<void(Args...)> func, Args ... args)
{
    // std::tuple<Args...> my_args(std::forward<Args>(args)...);
    // auto my_args(std::forward<Args>(args)...);
    // auto fun = func;
    auto f = hpx::util::bind(func, args...);

    _futures.reserve(_num_hpx_threads);
    th_ids.reserve(_num_hpx_threads);

    std::vector<hpx::future<void>> vec(_num_hpx_threads);

    for(int i=0; i<_num_hpx_threads; i++) {

        auto func_aux = [&, this](){
            th_ids.push_back(hpx::this_thread::get_id());
            std::invoke(f);
            //fun(my_args);
        };
        _futures.push_back(hpx::async(func_aux));

    }
    hpx::wait_all(_futures);

    _futures.clear();
    th_ids.clear();
}



}



#endif
