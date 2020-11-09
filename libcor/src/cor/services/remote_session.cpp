
//#include <hpx/hpx.hpp>

#include "cor/services/remote_session.hpp"
#include "cor/services/session_manager.hpp"

#include <iostream>

using namespace ssh;

// namespace cor {

// RemoteSession::RemoteSession(std::string const& host, std::string const& port, std::string const& cmd) :
//     _host{host},
//     _port{port},
//     _cmd{cmd},
//     _th_svc{},
//     _event{},
//     _sess{},
//     _ch{_sess},
//     _cin{_sess},
//     _cout{_sess},
//     _cerr{_sess},
//     _mtx{},
//     _cv{},
//     _done{false}
// {}

// RemoteSession::~RemoteSession() = default;

// void RemoteSession::Run()
// {
//     _th_svc = std::move(std::thread(&RemoteSession::operator(), this));
// }

// void RemoteSession::Wait()
// {
//     _th_svc.join();
// }

// void RemoteSession::operator()()
// {
//     try {
//         _sess.SetOption(SSH_OPTIONS_HOST, const_cast<char*>(_host.c_str()));
//         _sess.SetOption(SSH_OPTIONS_PORT_STR, const_cast<char*>(_port.c_str()));

//         _sess.Connect();
//         _sess.PublicKeyAuth();
//         _ch.OpenSession();
        
//         // stdin
//         _cin.SetOutChannel(_ch, SSH_CONNECTOR_STDOUT);
//         _cin.SetInFD(0);

//         // stdout
//         _cout.SetOutFD(1);
//         _cout.SetInChannel(_ch, SSH_CONNECTOR_STDOUT);

//         // stderr
//         _cerr.SetOutFD(2);
//         _cerr.SetInChannel(_ch, SSH_CONNECTOR_STDERR);

//         // set event loop
//         _event.AddConnector(_cin);
//         _event.AddConnector(_cout);
//         _event.AddConnector(_cerr);

//         // run command
//         _ch.RequestExec(_cmd);

//         _event.DoPoll();

//         auto exit_status = _ch.GetExitStatus();
//         if (exit_status)
//             std::cerr << "ERROR: job terminated with exit status " << exit_status << std::endl;

//         _event.RemoveConnector(_cin);
//         _event.RemoveConnector(_cout);
//         _event.RemoveConnector(_cerr);
        
//         _ch.SendEof();
//         _ch.Close();
//         //_sess.Disconnect();
//     } catch (SshException const& ex) {
//         std::cerr << "Error: " << ex.GetError() << "\n";
//         std::exit(EXIT_FAILURE);
//     }
// }

// }
