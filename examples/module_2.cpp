
#include "cor/cor.hpp"
typedef std::vector<std::string> tipo4;
extern "C"
{
    void Test(std::vector<std::string>);
}

void Test(std::vector<std::string> v) {

    cor::GetDomain().then(hpx::util::unwrapping([](auto domain){ 
        std::cout << domain->GetActiveResourceIdp().get() << "\n"; 
    })).get();

    for(int i=0; i<v.size(); i++) {
        std::cout << v[i] << std::endl;
    }
    std::cout << v.size() << std::endl;
    std::cout << "ola" << std::endl;
}
