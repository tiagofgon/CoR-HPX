#include "cor/cor.hpp"
#include "external/Rand.h"
#include "external/CpuTimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <omp.h>

extern "C"
{
    void Main(int argc);
}

int gcd(int u, int v)
{
if (v == 0) return u;
return gcd(v, u % v);
}

                                     
static size_t nAgents=1;                  //shared variables
static size_t initial,final;
static int *the_num, *num,  *den; 

std::shared_ptr<cor::Operon_Client> operon;

struct MapDynamic_object {
    void operator()() {
    
        auto rank = operon->GetRank();
        int beg, end;
        beg = initial;                    // initialize [beg, end) to global range      

        while(beg<final+1) {
            std::pair<int, int> par = operon->ScheduleDynamic(initial, final+1, 500).get();
            beg=par.first;
            end=par.second;
            int i, j, factor, ii, sum, done, n;

            for (i = beg; i < end; i++) {
                ii = i - initial;
                sum = 1 + i;
                the_num[ii] = i;
                done = i;
                factor = 2;
                while (factor < done) {
                    if ((i % factor) == 0) {
                        sum += (factor + (i/factor));
                        if ((done = i/factor) == factor) sum -= factor;
                    }
                    factor++;
                }
                num[ii] = sum; den[ii] = i;
                n = gcd(num[ii], den[ii]);
                num[ii] /= n;
                den[ii] /= n;
                //std::cout << ii << ": " << num[ii] << "/" << den[ii] << std::endl;
            } // end for
        }
    }
};
MapDynamic_object _MapDynamic;
hpx::function<void()> MapDynamic(_MapDynamic);
 

struct Map_object {
    void operator()() {
    
        auto rank = operon->GetRank();
        auto [beg, end] = operon->ScheduleStatic(initial, final+1).get();
        int i, j, factor, ii, sum, done, n;

        for (i = beg; i < end; i++) {
            ii = i - initial;
            sum = 1 + i;
            the_num[ii] = i;
            done = i;
            factor = 2;
            while (factor < done) {
                if ((i % factor) == 0) {
                    sum += (factor + (i/factor));
                    if ((done = i/factor) == factor) sum -= factor;
                }
                factor++;
            }
            num[ii] = sum; den[ii] = i;
            n = gcd(num[ii], den[ii]);
            num[ii] /= n;
            den[ii] /= n;
            //std::cout << ii << ": " << num[ii] << "/" << den[ii] << std::endl;
        } // end for
        
    }
};
Map_object _Map;
hpx::function<void()> Map(_Map);


struct Reduce_object {
    void operator()() {
        int last = final-initial+1;
        size_t start=0,endd=last;
    
       auto [beg, end] = operon->ScheduleStatic(start, endd).get();
        auto rank = operon->GetRank().get();
        // -- REDUCE --
        std::cout << " Reduce rank" << rank << "start: " << beg << "  " << end << std::endl;
    	int i, j, factor, ii, sum, done, n;

    	for (i = beg; i < end; i++) {
    		for (j = i+1; j< last; j++) {
    		  if ((num[i] == num[j]) && (den[i] == den[j]))
    		    printf ("%d and %d are FRIENDLY \n", the_num[i], the_num[j]);
    		}
            //std::cout << i << ": " << num[i] << "/" << den[i] << std::endl;
    	}
    }
};
Reduce_object _Reduce;
hpx::function<void()> Reduce(_Reduce);

 
void FriendlyNumbers ()
{
  int last = final-initial+1;
  
  the_num = new int[last];
  num = new int[last];
  den = new int[last];
  auto funcReport = [=] () { for (int z=0; z <last; z++) std::cout << the_num[z] << " " << num[z] << "/"<<den[z] << "\n";};
 //auto mutex = domain->CreateLocal<cor::Mutex>(domain->Idp(), "Guarda");
  // mutex->Acquire();
  // funcReport();
  // mutex->Release();
  auto domain = cor::GetDomain().get();
  std::size_t const& pool_size = nAgents;
  operon = domain->CreateLocal<cor::Operon_Client>(domain->Idp().get(),  "", pool_size).get();
  CpuTimer TRM,TRR, TRT;

  TRM.Start();
  TRT.Start();
    auto fut1 = operon->Dispatch(Map); //Activate MapStatic
  //auto fut1 = operon->Dispatch(MapDynamic); //Activate MapDynamic
  fut1.get();
    
  //funcReport();

  TRM.Stop();  
  //pool = new cor::Pool(nAgents);

  TRR.Start();
  auto fut2 = operon->Dispatch(Reduce);
  fut2.get();
  //Activate Reduce
  TRR.Stop();
  
  TRT.Stop();
  
  TRM.Report();
  TRR.Report();
  TRT.Report();
  
  delete[] the_num;
  delete[] num;
  delete[] den;
}


void Main(int argc)
{
   initial = 1;
   final = 100000;
   nAgents = 4;
  
      
    //ini = omp_get_wtime();
    FriendlyNumbers();
    // fim = omp_get_wtime();
    // printf("Tempo total %lf seconds\n",fim - ini);
}
