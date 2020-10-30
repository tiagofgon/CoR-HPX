// #ifndef COR_REMOTE_SESSION_HPP
// #define COR_REMOTE_SESSION_HPP

// #include "cor/external/libssh/libssh.hpp"

// #include <thread>
// #include <mutex>
// #include <condition_variable>

// namespace cor {

// class SessionManager;

// class RemoteSession
// {

// public:
//     explicit RemoteSession(std::string const& host, std::string const& port, std::string const& cmd);
//     ~RemoteSession();

//     RemoteSession() = delete;

//     RemoteSession(RemoteSession const&) = delete;
//     RemoteSession& operator=(RemoteSession const&) = delete;

//     RemoteSession(RemoteSession&&) noexcept = delete;
//     RemoteSession& operator=(RemoteSession&&) noexcept = delete;

//     void Run();
//     void Wait();

// private:
//     void operator()();

//     std::string _host;
//     std::string _port;
//     std::string _cmd;

//     std::thread _th_svc;

//     ssh::Event _event;
//     ssh::Session _sess;
//     ssh::Channel _ch;
//     ssh::Connector _cin;
//     ssh::Connector _cout;
//     ssh::Connector _cerr;

//     std::mutex _mtx;
//     std::condition_variable _cv;
//     bool _done;

// };

// }

// #endif
