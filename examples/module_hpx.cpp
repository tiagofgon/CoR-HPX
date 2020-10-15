
#include "cor/cor_hpx.hpp"

extern "C"
{
    void Main(idp_t rsc_idp);
}

void Main(idp_t rsc_idp) {

    auto domain = cor::GetDomain();
    std::cout << domain->GetActiveResourceIdp() << "\n";

}
