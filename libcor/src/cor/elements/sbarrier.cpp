#include "cor/elements/sbarrier.hpp"

namespace cor {

SBarrier::SBarrier() = default;

SBarrier::SBarrier(idp_t idp, idp_t clos) :
    _idp{idp},
    _clos{clos}
{
    //std::cout << "Criado um objeto da classe \"SBarrier\", com idp: " << _idp << std::endl;
}

SBarrier::~SBarrier() = default;

// SBarrier::SBarrier(SBarrier&&) noexcept = default;

// SBarrier& SBarrier::operator=(SBarrier&&) noexcept = default;


// Não faz nada
void SBarrier::Synchronize()
{
    // auto sorg = global::pod_object->GetLocalResource<cor::Closure_Client>(_clos);
    // std::size_t total_members = sorg->GetTotalMembers();
    // hpx::lcos::barrier barrier(std::to_string(_idp), total_members);
    // barrier.wait();
    
    // auto active_rsc_idp = global::pod->GetActiveResourceIdp();
    // auto sorg = global::pod->GetLocalResource<cor::StaticOrganizer>(_clos);
    // auto idm = sorg->GetIdm(active_rsc_idp);

    
    // global::pod->SynchronizeStaticGroup(_clos);
}

idp_t SBarrier::GetIdpClos() const {
    return _clos;
}

idp_t SBarrier::GetSBarrierIdp() const {
    return _idp;
}

}
