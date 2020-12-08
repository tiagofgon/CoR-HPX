
#include "cor/cor.hpp"

void Main(int input) {

    auto domain = cor::GetDomain();
    std::cout << domain->GetActiveResourceIdp() << "\n";

}
