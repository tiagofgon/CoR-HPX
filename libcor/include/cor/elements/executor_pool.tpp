#ifdef EXECUTOR_POOL_HPP

namespace cor
{

template < typename Func, typename ... Args >
void ExecutorPool::DispatchTemplated(Func && func, Args && ... args)
{
    for(int i=0; i<_num_hpx_threads; i++) {
        // std::cout << "ola" << std::endl;
        
        hpx::future<void> fut = hpx::async([](){
            std::cout << "nada" << std::endl;
        });
        _futures.emplace_back(std::move(fut));
    }

}


}



#endif
