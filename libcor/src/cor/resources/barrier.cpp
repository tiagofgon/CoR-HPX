#include "cor/resources/barrier.hpp"

namespace cor {

Barrier::Barrier() = default;

Barrier::Barrier(idp_t idp, idp_t clos) :
    base_type(idp),
    _sbarrier{idp, clos}
{
    std::cout << "Criado um componente \"Barrier\", com idp: " << idp << std::endl;
}

Barrier::~Barrier() = default;

void Barrier::Synchronize()
{
    return _sbarrier.Synchronize();
}

idp_t Barrier::GetIdpClos()
{
    return _sbarrier.GetIdpClos();
}

idp_t Barrier::GetSBarrierIdp()
{
    return _sbarrier.GetSBarrierIdp();
}

}


typedef cor::Barrier Barrier;
typedef hpx::components::component<Barrier> Barrier_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(Barrier_type, Barrier, "Resource");



/* SBarrier actions */
typedef cor::Barrier::Synchronize_action_Barrier Synchronize_action_Barrier;
typedef cor::Barrier::GetIdpClos_action_Barrier GetIdpClos_action_Barrier;
typedef cor::Barrier::GetSBarrierIdp_action_Barrier GetSBarrierIdp_action_Barrier;

HPX_REGISTER_ACTION(Synchronize_action_Barrier);
HPX_REGISTER_ACTION(GetIdpClos_action_Barrier);
HPX_REGISTER_ACTION(GetSBarrierIdp_action_Barrier);