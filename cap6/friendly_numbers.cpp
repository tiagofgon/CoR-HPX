/* ---------- 
-- Tiago Gonçalves - University of Minho, 2021 --

To run: ./corhpx apps ctx 1 0 ../cap6/libmcpi.so «number of hpx threads» «policy» «range of numbers»
examples:
    ./corhpx apps ctx 1 0 ../cap6/libfriendly_numbers.so 2 d 100
    ./corhpx apps ctx 1 0 ../cap6/libfriendly_numbers.so 16 s 1000000
---------- */

#include "cor/cor.hpp"
#include "external/Rand.h"
#include "external/CpuTimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <omp.h>

extern "C"
{
    void Main(int argc, char *argv[]);
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

void MapGuided() {

    auto rank = operon->GetRank();
    int beg, end;
    beg = initial;                    // initialize [beg, end) to global range      

    while(beg<final+1) {
        std::pair<int, int> par = operon->ScheduleGuided(initial, final+1, 500);
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

void MapDynamic() {

    auto rank = operon->GetRank();
    int beg, end;
    beg = initial;                    // initialize [beg, end) to global range      

    while(beg<final+1) {
        std::pair<int, int> par = operon->ScheduleDynamic(initial, final+1, 500);
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


void MapStatic() {

    auto rank = operon->GetRank();
    auto [beg, end] = operon->ScheduleStatic(initial, final+1);
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


void Reduce() {
    int last = final-initial+1;
    size_t start=0,endd=last;
    std::list< std::pair<int,int> > friendlynumberslist;

    auto [beg, end] = operon->ScheduleStatic(start, endd);
    auto rank = operon->GetRank();
    // -- REDUCE --
    //std::cout << " Reduce rank " << rank << "start: " << beg << "  " << end << std::endl;
    int i, j, factor, ii, sum, done, n;

    for (i = beg; i < end; i++) {
        for (j = i+1; j< last; j++) {
            if ((num[i] == num[j]) && (den[i] == den[j])){
                //friendlynumberslist.emplace_back(the_num[i],the_num[j]);
                //printf ("%d and %d are FRIENDLY \n", the_num[i], the_num[j]);
            }
        }
        //std::cout << i << ": " << num[i] << "/" << den[i] << std::endl;
    }
    // return friendlynumberslist;
}

 
void FriendlyNumbers(char policy)
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
    auto domain = cor::GetDomain();
    std::size_t const& pool_size = nAgents;
    operon = domain->CreateLocal<cor::Operon_Client>(domain->Idp(),  "", pool_size);
    CpuTimer TRM,TRR, TRT;

    TRM.Start();
    TRT.Start();
    hpx::future<void> fut;
    switch(policy)
    {
        case 's':
        fut = operon->Dispatch(MapStatic); //Activate MapStatic
        break;

        case 'd':
        fut = operon->Dispatch(MapDynamic); //Activate MapStatic
        break;

        case 'g':
        fut = operon->Dispatch(MapGuided); //Activate MapGuided
        break;

        default:
        std::cout << "wrong policy" << std::endl;
        return;
    }

    fut.get();
        
    //funcReport();

    TRM.Stop();  
    //pool = new cor::Pool(nAgents);

    TRR.Start();
    auto fut2 = operon->Dispatch(Reduce); // static
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


void Main(int argc, char *argv[])
{
   initial = 1;
   final = 100000;
   char policy = 's';
   if (argc >= 1) nAgents = std::atoi(argv[0]);
   if (argc >= 2) policy = argv[1][0];
   if (argc == 3) final = std::atoi(argv[2]);
  
      
    //ini = omp_get_wtime();
    FriendlyNumbers(policy);
    // fim = omp_get_wtime();
    // printf("Tempo total %lf seconds\n",fim - ini);
}
