#ifndef COR_IDP_MANAGER_HPP
#define COR_IDP_MANAGER_HPP

#include <hpx/hpx.hpp>
#include <hpx/include/actions.hpp>
#include <hpx/include/components.hpp>

#include "cor/system/macros.hpp"


namespace cor {

// Garante a sincronização entre actions
struct IdpManager : public hpx::components::locking_hook<hpx::components::component_base<IdpManager>>
{

public:
    ~IdpManager();
    IdpManager();

    page_t GeneratePage();

    HPX_DEFINE_COMPONENT_ACTION(IdpManager, GeneratePage, GeneratePage_action_IdpManager);

private:
    idp_t _ids_capacity; // numero total de idps
    // idp_t _ids_counter; // contador de idps


    page_t _next_page;
    page_t _current_page;
    page_t _page_size;
    page_t _ids_counter;

};

}

typedef cor::IdpManager::GeneratePage_action_IdpManager GeneratePage_action_IdpManager;

HPX_REGISTER_ACTION_DECLARATION(GeneratePage_action_IdpManager);


#endif
