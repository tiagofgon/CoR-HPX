#include "cor/resources/closure_client.hpp"
#include "cor/system/system.hpp"
#include "cor/system/pod_client.hpp"

namespace cor {


void Closure_Client::Migrar(hpx::id_type dest)
{
    global::pod->Migrar<cor::Closure>(this->get_id(), dest);
}

}
