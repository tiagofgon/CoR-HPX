// Tiago Gonçalves & António Pina - University of Minho - LIP, 2021
//
// This is a purely local version demonstrating how to print
// the active resource idp, synchronously and asynchronously
//
// To run: ./corhpx apps ctx 1 1 0 ../examples/libmodule.so
//

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

void Main(int argc, char *argv[]) {

    auto domain = cor::GetDomain();
    std::cout << domain->GetActiveResourceIdp() << " (sync mode)" << std::endl; 
    std::cout << domain->GetActiveResourceIdp(hpx::launch::async).get() << " (async mode)" << std::endl; 

}
