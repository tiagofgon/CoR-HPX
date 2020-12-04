#include "cor/system/system.hpp"
#include "cor/system/pod_client.hpp"
#include "cor/elements/executor_pool.hpp"

//#include "cor/cor.hpp"

#include <hpx/hpx.hpp>


namespace cor
{
    ExecutorPool::ExecutorPool(idp_t idp, std::size_t num_hpx_threads) :
        _idp{idp},
        _num_hpx_threads{num_hpx_threads},
        _fct{nullptr},
        _arg{nullptr},
        barrier{num_hpx_threads}
    {
        std::cout << "Criado um componente \"ExecutorPool\", com idp: " << idp << ", com threads: " << _num_hpx_threads << std::endl;
        // auto domain = cor::GetDomain();
        // auto agent_idp = domain->GetActiveResourceIdp();
        // std::string const& path = "";
        // auto group = domain->CreateLocal<cor::Group_Client>(domain->Idp(), "", path);
        // _group = group->Idp();

        _futures.reserve(num_hpx_threads);
        th_ids.reserve(num_hpx_threads);

        index=0;
        // for (std::size_t i = 0; i < num_hpx_threads; ++i) {
        //     auto agent = domain->CreateLocal<cor::ProtoAgent<void(void*)>>(group->Idp(), "", AgentFunc);
        //     agent->Run((void*)this);
        //     _agents.push_back(agent->Idp());
        // }
    }

    ExecutorPool::~ExecutorPool()
    {        

    }

    void ExecutorPool::Dispatch(void (*funct)(void *), void *arg)
    {
        // std::cout << "ola3" << std::endl;
        // std::cout << "_num_hpx_threads: " << _num_hpx_threads << std::endl;


        for(int i=0; i<_num_hpx_threads; i++) {
            // std::cout << "ola" << std::endl;
            
                hpx::future<void> fut = hpx::async([this, &funct, &arg](){
                    th_ids.push_back(hpx::this_thread::get_id());
                    // std::cout << "thread_id_1: " << hpx::this_thread::get_id() << std::endl;
                    // std::cout << "aux_2: " << aux << std::endl;
                    barrier.wait();
                    funct(arg);
                });
            
            _futures.push_back(std::move(fut));
        }
        
        hpx::wait_all(_futures);
        //_futures[0].wait();
    }


// ------------------------------------------------------------
// This function will be called by worker threads if they need
// to know the rank of the thread that is executing the job.
// This is important to distribute work among worker thread.
// Ranks are in [1, nTh].
// ------------------------------------------------------------
int ExecutorPool::GetRank()
{
   hpx::thread::id my_id; 
   int n, my_rank;

   my_id = hpx::this_thread::get_id();          // determine who  am
    
    n = 0;
    do
    {
    n++;
    } while(my_id != th_ids[n] && n<_num_hpx_threads); 

   if(n<=_num_hpx_threads) my_rank = n;    // OK, return rank
   else my_rank = 0;     // else, return error
   return my_rank;
}


void ExecutorPool::WaitForIdle()
{
    //std::cout << "hpx::wait_all" << std::endl;
    // hpx::wait_all(_futures);
    //hpx::when_all(_futures).get();
    //_futures[0].get();
    //_barrier->WaitForIdle();  
}

int ExecutorPool::GetNumThreads() {
    return _num_hpx_threads;
}


std::pair<int,int> ExecutorPool::ScheduleStatic(int Beg, int End) {
   int n, rank, beg, end;
   int size, D, R;
   rank = GetRank();

   size = End-Beg;
   D = (size/_num_hpx_threads);
   R = size%_num_hpx_threads;

   end = Beg;
   for(n=1; n<=rank; n++)
   {
      beg = end;
      end = beg+D;
      if(R)
      {
         end++;
         R--;
      }
   }
   Beg = beg;
   End = end;
   return std::make_pair (Beg,End);
}

std::vector<std::pair<int,int>> ExecutorPool::ScheduleStatic(int Beg, int End, int chunk)
{
   int n, rank, beg, end;
   int size, D, R;
   rank = GetRank();
   
   size = End-Beg;
   int nchunks = (size/chunk); // numero de chunks
   R = size%chunk; // tamanho do chunk que resta, caso nao seja multiplo
   std::vector< std::vector<std::pair<int,int>> > res(_num_hpx_threads+1); // +1 porque o rank começa no 1

   end = Beg;
   int pos = 1;
   for(n=0; n<nchunks; n++)
   {
      pos = (n % _num_hpx_threads)+1; // calcular a posição do array para a thread respetiva
      beg = end;
      end = beg+chunk;
      res[pos].push_back(std::make_pair (beg,end));
   }
   if(R!=0) {
      pos = (n % _num_hpx_threads)+1;
      beg = end;
      end = beg+R;
      res[pos].push_back(std::make_pair (beg,end));
   }

   return res[rank];
}




std::pair<int,int> ExecutorPool::ScheduleDynamic(int Beg, int End, int chunk)
   {
   int beg, end;

   beg = index.fetch_add(chunk);
   if(beg >= End) 
      return std::make_pair (beg,beg);
   end = beg+chunk;
   if(end > End) 
      end = End;

   return std::make_pair(beg,end);
   }

std::pair<int,int> ExecutorPool::ScheduleGuided(int Beg, int End, int chunk)
   {
   int n, rank, beg, end;
   int size, D, R;
   rank = GetRank();

   {
   std::lock_guard<hpx::mutex> lock(_mtx);
   size = End - index;
   D = (size/_num_hpx_threads);
   if(D < chunk) 
      D = chunk;
   beg = index.fetch_add(D);
   }
   if(beg >= End) 
      return std::make_pair (beg,beg);
   end = beg+D;
   if(end > End) 
      end = End;
   

   return std::make_pair (beg,end);
   }

}
