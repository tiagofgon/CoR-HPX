#include "cor/services/idp_manager.hpp"

namespace cor {

IdpManager::IdpManager()
{
    _page_size = 0x100;
    _next_page = 0xffffff00;
}

IdpManager::~IdpManager() = default;

page_t IdpManager::GeneratePage()
{
    // generate new page
    auto page = _next_page;
    _next_page -= _page_size;

    return page;
}


}

// HPX_REGISTER_COMPONENT_MODULE();

typedef cor::IdpManager IdpManager;
typedef hpx::components::component<cor::IdpManager> IdpManager_type;

HPX_REGISTER_COMPONENT(IdpManager_type, IdpManager);

typedef cor::IdpManager::GeneratePage_action_IdpManager GeneratePage_action_IdpManager;

HPX_REGISTER_ACTION(GeneratePage_action_IdpManager);
