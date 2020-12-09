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
    void Main(int argc);
}


struct piCalc_static {
    void operator()() {
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
};
piCalc_static _piCalc_static;
hpx::function<void()> McPi_static(_piCalc_static);




struct piCalc_static_chunk {
    void operator()() {
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
};
piCalc_static_chunk _piCalc_static_chunk;
hpx::function<void()> McPi_static_chunk(_piCalc_static_chunk);



struct piCalc_static_dynamic {
    void operator()() {
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
};
piCalc_static_dynamic _piCalc_static_dynamic;
hpx::function<void()> McPi_static_dynamic(_piCalc_static_dynamic);


struct Func {
    void operator()(std::vector<int> v, int a) {
            std::cout << a << " " << v[0] << std::endl;

    }
};
Func func_aux;
hpx::function<void(std::vector<int>, int)> func(func_aux);




auto calc_pi = [](){
    std::atomic<unsigned long> sum=0;

    // The interior of the vector is handled with a parallel for loop. HPX
    // implicitly creates a task for each chunk of the input range. HPX does not
    // create one task for each index.
    hpx::for_loop(hpx::execution::par, 0, nsamples,
        [&](auto i) {
            auto x = R.draw();
            auto y = R.draw();
            if ((x*x+y*y) <= 1.0)
                ++sum;
        });

    double pi = 4.0 * double(sum)/nsamples;
    std::cout << "\nValue of PI McPi_chunk = " << pi << std::endl;
};






void Main(int argc)
{
    auto domain = cor::GetDomain().get();
    CpuTimer T;
    double pi, result = 0;
    std::size_t const& pool_size = 4;

    operon = domain->CreateLocal<cor::Operon_Client>(domain->Idp(),  "", pool_size);

    /* -------------- */


    // T.Start();
    // auto fut1 = operon->Dispatch(McPi_static);
    // fut1.get();
    // pi = 4.0 * double(sum_static)/nsamples;
    // std::cout << "\nValue of PI McPi_static = " << pi << std::endl;
    // T.Stop();
    // T.Report();


    // /* -------------- */


    // T.Start();
    // sum_static = 0;
    // auto fut2 = operon->Dispatch(McPi_static_chunk);
    // fut2.get();
    // pi = 4.0 * double(sum_static)/nsamples;
    // std::cout << "\nValue of PI McPi_static_chunk = " << pi << std::endl;
    // T.Stop();
    // T.Report();


    // /* -------------- */


    // T.Start();
    // sum_static = 0;
    // auto fut3 = operon->Dispatch(McPi_static_dynamic);
    // fut3.get();
    // pi = 4.0 * double(sum_static)/nsamples;
    // std::cout << "\nValue of PI McPi_static_dynamic = " << pi << std::endl;
    // T.Stop();
    // T.Report();


    // /* -------------- */

    // int a = 1;
    // std::vector<int> v = {1, 2};
    // operon->Dispatch(func, v, 1).get();

T.Start();
    calc_pi();
T.Stop();
T.Report();
}