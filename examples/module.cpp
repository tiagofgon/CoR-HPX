
#include "cor/cor.hpp"

extern "C"
{
    void Main(int rsc_idp);
}

void Main(int input) {

    auto domain = cor::GetDomain().get();
    std::cout << domain->GetActiveResourceIdp() << "\n";

}
