/* ---------- 
-- Tiago Gonçalves - University of Minho, 2021 --

To run: ./corhpx apps ctx 1 0 ../cap6/libmcpi.so «number of hpx threads» «number of samples»
---------- */

#include "cor/cor.hpp"

#include "external/CpuTimer.h"
#include "external/Rand.h"

#include <vector>
#include <numeric>

std::vector<long> accepted;
long nsamples = 1000000000;

std::atomic<unsigned long> sum_static=0;


Rand R(999);

std::shared_ptr<cor::Operon_Client> operon;

extern "C"
{
    void Main(int argc, char *argv[]);
}

void McPi_static() {
//std::cout << "McPi_static " << std::endl;
    unsigned long ct;
    double x, y;
    ct = 0;
    auto [beg, end] = operon->ScheduleStatic(0, nsamples);
    for (auto n = beg; n < end; ++n) {
        x = R.draw();
        y = R.draw();
        if ((x*x+y*y) <= 1.0)
            ++ct;
    }
    sum_static.fetch_add(ct);
}

void McPi_static_chunk() {
    //std::cout << "McPi_static_chunk " << std::endl;
    unsigned long ct;
    double x, y;
    ct = 0;
    auto vec = operon->ScheduleStatic(0, nsamples, 500);
    for(int i=0; i<vec.size(); i++){
        auto [beg, end] = vec[i];
        for (auto n = beg; n < end; ++n) {
            x = R.draw();
            y = R.draw();
            if ((x*x+y*y) <= 1.0)
                ++ct;
        }
    }

    sum_static.fetch_add(ct);
}

void McPi_dynamic() {
    //std::cout << "McPi_static_chunk " << std::endl;
    unsigned long ct;
    double x, y;
    ct = 0;
    int beg, end;
    beg = 0;                    // initialize [beg, end) to global range
    end = nsamples;

    while(beg<nsamples) {
        std::pair<int, int> par = operon->ScheduleDynamic(0, nsamples, 500);
        beg=par.first;
        end=par.second;
        for(auto n = beg; n < end; ++n) {
            x = R.draw();
            y = R.draw();
            if ((x*x+y*y) <= 1.0)
                ++ct;
        }
    }

    sum_static.fetch_add(ct);
}


void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();
    CpuTimer T;
    double pi, result = 0;
    std::size_t pool_size = 4;

    if (argc >= 1) pool_size = std::atoi(argv[0]);
    if (argc == 2) nsamples = atoi(argv[1]);

    operon = domain->CreateLocal<cor::Operon_Client>(domain->Idp(),  "", pool_size);

    /* -------------- */


    T.Start();
    auto fut1 = operon->Dispatch(McPi_static);
    fut1.get();
    pi = 4.0 * double(sum_static)/nsamples;
    std::cout << "\nValue of PI McPi_static = " << pi << std::endl;
    T.Stop();
    T.Report();


    /* -------------- */

    T.Start();
    sum_static = 0;
    auto fut2 = operon->Dispatch(McPi_static_chunk);
    fut2.get();
    pi = 4.0 * double(sum_static)/nsamples;
    std::cout << "\nValue of PI McPi_static_chunk = " << pi << std::endl;
    T.Stop();
    T.Report();


    /* -------------- */


    T.Start();
    sum_static = 0;
    auto fut3 = operon->Dispatch(McPi_dynamic);
    fut3.get();
    pi = 4.0 * double(sum_static)/nsamples;
    std::cout << "\nValue of PI McPi_dynamic = " << pi << std::endl;
    T.Stop();
    T.Report();


    /* -------------- */


}