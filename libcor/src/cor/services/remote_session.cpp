
//#include <hpx/hpx.hpp>

#include "cor/services/remote_session.hpp"
#include "cor/services/session_manager.hpp"

#include <iostream>

using namespace ssh;

namespace cor {

RemoteSession::RemoteSession(std::string const& host, std::string const& port, std::string const& cmd) :
    _host{host},
    _port{port},
    _cmd{cmd},
    _th_svc{},
    _event{},
    _sess{},
    _ch{_sess},
    _cin{_sess},
    _cout{_sess},
    _cerr{_sess},
    _mtx{},
    _cv{},
    _done{false}
{}

RemoteSession::~RemoteSession() = default;

void RemoteSession::Run()
{
    _th_svc = std::move(std::thread(&RemoteSession::operator(), this));
    // future = hpx::async([this](){
    //     this->operator()();
    // });

    // hpx::apply([this](){
    //     this->operator()();
    // });
}

void RemoteSession::Wait()
{
    _th_svc.join();
    // future.wait();
}

void RemoteSession::operator()()
{
    try {
        _sess.SetOption(SSH_OPTIONS_HOST, const_cast<char*>(_host.c_str()));
        _sess.SetOption(SSH_OPTIONS_PORT_STR, const_cast<char*>(_port.c_str()));

        _sess.Connect();
        std::cout << "Connect()" << std::endl;
        _sess.PublicKeyAuth();
        _ch.OpenSession();
        
        // stdin
        _cin.SetOutChannel(_ch, SSH_CONNECTOR_STDOUT);
        _cin.SetInFD(0);

        // stdout
        _cout.SetOutFD(1);
        _cout.SetInChannel(_ch, SSH_CONNECTOR_STDOUT);

        // stderr
        _cerr.SetOutFD(2);
        _cerr.SetInChannel(_ch, SSH_CONNECTOR_STDERR);
        std::cout << "Connect()2" << std::endl;
        // set event loop
        _event.AddConnector(_cin);
        _event.AddConnector(_cout);
        _event.AddConnector(_cerr);

        // run command
        _ch.RequestExec(_cmd);
        std::cout << "Connect()3" << std::endl;
        _event.DoPoll();
        std::cout << "Connect()4" << std::endl;
        auto exit_status = _ch.GetExitStatus();
        if (exit_status)
            std::cerr << "ERROR: job terminated with exit status " << exit_status << std::endl;
        std::cout << "Connect()5" << std::endl;
        _event.RemoveConnector(_cin);
        _event.RemoveConnector(_cout);
        _event.RemoveConnector(_cerr);
        std::cout << "Connect6()" << std::endl;
        _ch.SendEof();
        _ch.Close();
        std::cout << "Connect()7" << std::endl;
        //_sess.Disconnect();
    } catch (SshException const& ex) {
        std::cerr << "Error: " << ex.GetError() << "\n";
        std::exit(EXIT_FAILURE);
    }
}

}
