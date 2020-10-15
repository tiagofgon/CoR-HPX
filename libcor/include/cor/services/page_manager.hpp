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
    // PageManager(Controller* ctrl);
    // explicit PageManager(std::shared_ptr<cor::Controller> ctrl);

    PageManager();
    PageManager(Controller* ctrl);
    PageManager(std::shared_ptr<cor::Controller> ctrl);

    // PageManager(Controller* ctrl);
    // PageManager(std::shared_ptr<cor::Controller> ctrl);
    
    ~PageManager();

    void CreateInitialContext();

    idp_t GenerateIdp();
    void UpdatePage(page_t page);

    // PageManager() = delete;
    PageManager(PageManager const&) = delete;
    PageManager& operator=(PageManager const&) = delete;
    PageManager(PageManager&&) = delete;
    PageManager& operator=(PageManager&&) = delete;

private:
    // std::shared_ptr<cor::Controller> _ctrl2;
    // Controller* _ctrl;

    std::shared_ptr<cor::Controller> _ctrl_new2;
    Controller* _ctrl;

    bool _is_main_mgr;

    page_t _current_page;
    page_t _page_size;
    page_t _ids_counter;

    std::mutex _mtx;
    std::condition_variable _cv;
};

}

#endif
