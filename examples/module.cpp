
#include "cor/cor.hpp"


extern "C"
{
    void Main(int input);
}

void Main(int input) {

    auto domain = cor::GetDomain();
    std::cout << domain->GetActiveResourceIdp() << "\n";

}
