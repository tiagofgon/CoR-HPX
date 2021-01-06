/* ---------- 
-- Tiago Gonçalves - University of Minho, 2021 --

To run: ./corhpx apps ctx 1 0 ../examples/libmodule.so
---------- */

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

void Main(int argc, char *argv[]) {

    auto domain = cor::GetDomain();
    std::cout << domain->GetActiveResourceIdp() << " -> sync" << std::endl; 
    std::cout << domain->GetActiveResourceIdp(hpx::launch::async).get() << " -> async" << std::endl; 

}
