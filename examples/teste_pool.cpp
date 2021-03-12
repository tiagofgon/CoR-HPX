/* ---------- 
-- Tiago Gonçalves - University of Minho, 2021 --

To run: ./corhpx apps ctx 1 0 ../examples/libteste_pool.so 4(number of hpx threads)
---------- */


#include "cor/cor.hpp"

std::shared_ptr<cor::Operon_Client> operon;

#define VECSIZE 60

std::vector<char> thread1(VECSIZE, ' ');
std::vector<char> thread2(VECSIZE, ' ');
std::vector<char> thread3(VECSIZE, ' ');
std::vector<char> thread4(VECSIZE, ' ');

//Mostra onde cada thread esteve a trabalhar no intervalo de dados
void print_schedule() {
   std::cout << "Thread 1:";
   for(int n=0; n<VECSIZE; n++){
      std::cout << thread1[n];
   }
   std::cout << "|" << std::endl;
   std::cout << "Thread 2:";
   for(int n=0; n<VECSIZE; n++){
      std::cout << thread2[n];
   }
   std::cout << "|" << std::endl;
   std::cout << "Thread 3:";
   for(int n=0; n<VECSIZE; n++){
      std::cout << thread3[n];
   }
   std::cout << "|" << std::endl;
   std::cout << "Thread 4:";
   for(int n=0; n<VECSIZE; n++){
      std::cout << thread4[n];
   }
   std::cout << "|" << std::endl;
}


// Função que simula trabalho de I/O, e guarda no array da thread respetiva
// em que posição está a trabalhar.
inline void doWork(int beg, int end){
   auto rank = operon->GetRank();
   for(int n=beg; n<end; n++){
      switch (rank)
      {
         case 1:
         thread1[n] = '*';
         break;

         case 2:
         thread2[n] = '*';
         break;

         case 3:
         thread3[n] = '*';
         break;

         case 4:
         thread4[n] = '*';
         break;

         default:
         ;
      }
   }
}


void funcStatic()
{
   int beg, end;
   beg = 0;                    // initialize [beg, end) to global range
   end = VECSIZE;
   std::pair<int, int> par = operon->ScheduleStatic(0, VECSIZE);
   beg=par.first;
   end=par.second;
   //std::cout << "\n Thread: " << operon->GetRank() << " " << "computing in range [" << beg << " , " << end << ")" << std::endl;
   // std::cout << "beg: " << beg << std::endl;
   doWork(beg, end);
}

// void funcDynamic(void *arg)
// {
//     int beg, end;
//     beg = 0;                    // initialize [beg, end) to global range
//     end = VECSIZE;

//     while(beg<VECSIZE) {
//         std::pair<int, int> par = operon->ScheduleDynamic(0, VECSIZE, 7);
//         beg=par.first;
//         end=par.second;
//         //std::cout << "\n Thread: " << TH->GetRank() << " " << "computing in range [" << beg << " , " << end << ")" << std::endl;
//         //std::cout << "beg: " << beg << " end: " << end << std::endl;
//         doWork(beg, end);
//     }
// }

// void funcGuided(void *arg)
// {
//     int beg, end;
//     beg = 0;                    // initialize [beg, end) to global range
//     end = VECSIZE;
    
//       while(beg<VECSIZE) {
//          std::pair<int, int> par = operon->ScheduleGuided(0, VECSIZE, 7);
//          beg=par.first;
//          end=par.second;
//          //std::cout << "\n Thread: " << TH->GetRank() << " " << "computing in range [" << beg << " , " << end << ")" << std::endl;
//          //std::cout << "beg: " << beg << " end: " << end << std::endl;
//          doWork(beg, end);

//       }
// }

extern "C"
{
    void Main(int argc, char *argv[]);
}

 

void Main(int argc, char *argv[]) {
   std::size_t pool_size = 1;
   if (argc >= 1) pool_size = std::atoi(argv[0]);

   auto domain = cor::GetDomain();
   auto agent_idp = domain->GetActiveResourceIdp();
   auto clos_idp = domain->GetPredecessorIdp(agent_idp);
   auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
   auto clos_size = clos->GetTotalMembers();
   auto rank = clos->GetIdm(agent_idp);
   auto parent_idp = clos->GetParent();

   auto myrankfunc = [&](){
      std::cout << operon->GetRank() << std::endl;
   };

   operon = domain->CreateLocal<cor::Operon_Client>(domain->Idp(),  "", pool_size);

   auto res = operon->Dispatch(funcStatic);
   res.get();
    // // operon->Dispatch(funcDynamic, nullptr);
    // //operon->Dispatch(funcGuided, nullptr);
    // //operon->WaitForIdle();

    print_schedule();

}
