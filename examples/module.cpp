
#include "cor/cor.hpp"

extern "C"
{
    void Main(int rsc_idp);
}

void Main(int input) {

    cor::GetDomain().then(hpx::util::unwrapping([](auto domain){ 
        std::cout << domain->GetActiveResourceIdp().get() << "\n"; 
    })).get();

}
