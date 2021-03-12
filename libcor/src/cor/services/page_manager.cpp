#include "cor/services/page_manager.hpp"
#include "cor/services/controller.hpp"

#include <cassert>

namespace cor {

PageManager::PageManager(Controller* ctrl) :
    _ctrl{ctrl}
{
    //std::cout << "PageManager contrutor1" << std::endl;
}

void PageManager::CreateInitialContext()
{
    _page_size = 0x100;
    _ids_counter = 0x100;
    
}

idp_t PageManager::GenerateIdp()
{
    // lock to access pages manager variables
    
    std::unique_lock<std::mutex> lk(_mtx);
    
    // verify if a new id can be generated,
    // using while to prevent the case that are more threads
    // waiting than the number of ids_counter
    // and avoiding spurious wakeups
    // std::cout << "aqui " << std::endl;
    // std::cout << "ali " << std::endl;

    if(_ids_counter >= _page_size) {
        // request a page
        page_t new_page = _ctrl->RequestPage();
        _current_page = new_page;
        _ids_counter = 0;
           
    }
    
    // new id, increment ids counter
    auto id = _current_page - _ids_counter;
    ++_ids_counter;
    // std::cout << "page_mananger.cpp" << std::endl;
    return id;
}


}
