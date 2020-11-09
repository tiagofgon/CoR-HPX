#include "cor/services/session_manager.hpp"
#include "cor/services/remote_session.hpp"

// #include <thread>
// #include <algorithm>

// using namespace std::chrono_literals;

// namespace cor {

// SessionManager::SessionManager() :
//     _sessions{},
//     _mtx{}
// {}

// SessionManager::~SessionManager()
// {
//     std::for_each(_sessions.begin(), _sessions.end(), [](RemoteSession *rs) -> void { delete rs; });
// }

// void SessionManager::StopService()
// {
//     std::unique_lock<std::mutex> lk(_mtx);

//     for (auto rs: _sessions)
//         rs->Wait();
// }

// void SessionManager::CreateRemoteSession(std::string const& host, std::string const& port, std::string const& cmd)
// {
//     auto rs = new RemoteSession(host, port, cmd);
//     rs->Run();

//     {
//         std::unique_lock<std::mutex> lk(_mtx);
//         _sessions.push_back(rs);
//     }
// }

// }
