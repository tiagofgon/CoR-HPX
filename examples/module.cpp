
#include "cor/cor.hpp"
#include "utils.hpp"

extern "C"
{
    void Main(int input);
    idp_t ServerFunction(idp_t idp);
}


idp_t ServerFunction(idp_t domain_idp)
{
    auto domain = cor::GetDomain();
    std::string module = "libparallel_remote.so";
    std::string function = "ClientFunction";
    std::string const& modulee = module;
    std::string const& functionn = function;

    std::cout << "ENTREI em ServerFunction" << "\n";
    auto rsc_idp = domain->Create<cor::Agent_Client<void(idp_t)>>(domain_idp, "", modulee, functionn);
    idp_t const& rsc_idpp = rsc_idp;
    domain->Run<cor::Agent_Client<void(idp_t)>>(rsc_idpp, rsc_idpp);
    domain->Wait<cor::Agent_Client<void(idp_t)>>(rsc_idpp);
    domain->Get<cor::Agent_Client<void(idp_t)>>(rsc_idpp);
    return rsc_idp;
    //return domain_idp;
}


void Main(int input) {

    auto domain = cor::GetDomain();
    std::cout << domain->GetActiveResourceIdp() << "\n";

}
