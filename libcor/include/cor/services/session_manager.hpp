#ifndef COR_SESSION_MANAGER_HPP
#define COR_SESSION_MANAGER_HPP

#include <vector>
#include <mutex>
#include <hpx/hpx.hpp>

namespace cor {

class RemoteSession;

class SessionManager
{

public:
    SessionManager();
    ~SessionManager();

    SessionManager(SessionManager const&) = delete;
    SessionManager& operator=(SessionManager const&) = delete;

    SessionManager(SessionManager&&) noexcept = delete;
    SessionManager& operator=(SessionManager&&) noexcept = delete;

    void StopService();

    void CreateRemoteSession(std::string const& host, std::string const& port, std::string const& cmd);

private:
    std::vector<RemoteSession*> _sessions;
    std::mutex _mtx;

};

}

#endif
