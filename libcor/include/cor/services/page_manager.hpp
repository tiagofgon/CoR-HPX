#ifndef COR_PAGE_MANAGER_HPP
#define COR_PAGE_MANAGER_HPP

#include <mutex>
#include <condition_variable>
#include <string>

#include "cor/system/macros.hpp"
// #include "cor/services/controller.hpp"
// #include "cor/services/controller.hpp"

namespace cor {

class Controller;
// class Controller;

class PageManager
{

public:
    PageManager();
    PageManager(Controller* ctrl);
    PageManager(std::shared_ptr<cor::Controller> ctrl);
    ~PageManager() = default;

    void CreateInitialContext();

    idp_t GenerateIdp();

    // PageManager() = delete;
    PageManager(PageManager const&) = delete;
    PageManager& operator=(PageManager const&) = delete;
    PageManager(PageManager&&) = delete;
    PageManager& operator=(PageManager&&) = delete;

private:
    Controller* _ctrl;

    page_t _current_page;
    page_t _page_size;
    page_t _ids_counter;

    std::mutex _mtx;
};

}

#endif
