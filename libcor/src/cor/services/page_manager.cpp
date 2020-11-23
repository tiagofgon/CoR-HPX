#include "cor/services/page_manager.hpp"
#include "cor/services/controller.hpp"

#include <cassert>

namespace cor {

PageManager::PageManager(Controller* ctrl) :
    _ctrl{ctrl}
{
    //std::cout << "PageManager contrutor1" << std::endl;
}

PageManager::~PageManager() = default;

void PageManager::CreateInitialContext()
{
    _page_size = 0x100;
    _ids_counter = 0x100;
    

    // if (_is_main_mgr) {
    //     _current_page = 0xffffff00;
    //     _next_page = 0xfffffe00;
    //     _ids_counter = 0;
    // } else {
    //     _ids_counter = 0x100;
    // }

}

// idp_t PageManager::GenerateIdp()
// {
//     // lock to access pages manager variables
//     std::unique_lock<std::mutex> lk(_mtx);

//     // verify if a new id can be generated,
//     // using while to prevent the case that are more threads
//     // waiting than the number of ids_counter
//     // and avoiding spurious wakeups
//     while (_ids_counter >= _page_size) {
//         // if is the main manager then generate new page
//         if (_is_main_mgr) {
//             _current_page = _next_page;
//             _next_page -= _page_size;
//             _ids_counter = 0;
//         } else {
//             // verify if a page has already been requested
//             if (!_page_requested) {
//                 // request a page
//                 _ctrl->RequestPage();
//                 _page_requested = true;
//             }

//             // wait until the page is been updated
//             _cv.wait(lk);
//         }
//     }

//     // new id, increment ids counter
//     auto id = _current_page - _ids_counter;
//     ++_ids_counter;
//     std::cout << "page_mananger.cpp" << std::endl;
//     return id;
// }

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


// O pedido _ctrl->RequestPage() irá chamar esta função, que irá retornar a nova página
void PageManager::UpdatePage(page_t page)
{   
    // // lock to access pages manager variables
    // // std::lock_guard<std::mutex> lk(_mtx);
    
    // // update ids_counter and current_page
    // _current_page = page;
    // _ids_counter = 0;
    // std::cout << "page " << page << std::endl;
    // // notify new page event
    //     std::this_thread::sleep_for(std::chrono::seconds(1));
    // _cv.notify_all();
}

}
