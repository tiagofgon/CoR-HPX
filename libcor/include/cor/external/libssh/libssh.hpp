#ifndef COR_LIBSSH_HPP
#define COR_LIBSSH_HPP

#include <string>
#include <chrono>

#include <libssh/libssh.h>

namespace ssh
{

    class SshException
    {
    
    public:
        SshException(ssh_session session) :
            _error_code{ssh_get_error_code(session)},
            _error{ssh_get_error(session)}
        {}
 
        int GetErrorCode() const
        {
            return _error_code;
        }

        const char* GetError() const
        {
            return _error.c_str();
        }

    protected:
        int _error_code;
        std::string _error;

    };

    class Session
    {

    friend class Channel;
    friend class Connector;
    friend class SshException;

    public:
        Session() :
            _session{ssh_new()}
        {}

        Session(Session const&) = delete;
        Session& operator=(Session const&) = delete;

        Session(Session&&) noexcept = default;
        Session& operator=(Session&&) noexcept= default;

        ~Session()
        {
            ssh_free(_session);
            _session = nullptr;
        }

        void SetOption(enum ssh_options_e type, void *option) const
        {
            auto rc = ssh_options_set(_session, type, option);
            if (rc != SSH_OK)
                throw SshException(_session);
        }

        void Connect() const
        {
            auto rc = ssh_connect(_session);
            if (rc != SSH_OK)
                throw SshException(_session);
        }

        void Disconnect() const
        {
            ssh_disconnect(_session);
        }

        void PublicKeyAuth() const
        {
            auto rc = ssh_userauth_publickey_auto(_session, nullptr, nullptr);
            if (rc != SSH_OK)
                throw SshException(_session);
        }

        socket_t getSocket() const
        {
            return ssh_get_fd(_session);
        }

    private:
        ssh_session GetSession() const
        {
            return _session;
        }

        ssh_session _session;

    };

    class Channel
    {

    friend class Connector;

    public:
        Channel() = delete;

        Channel(Session& session) :
            _channel{ssh_channel_new(session.GetSession())},
            _session{&session}
        {}

        Channel(Channel const&) = delete;
        Channel& operator=(Channel const&) = delete;

        Channel(Channel&&) noexcept = default;
        Channel& operator=(Channel&&) noexcept= default;

        ~Channel()
        {
            ssh_channel_free(_channel);
            _channel = nullptr;
        }

        void OpenSession() const
        {
            auto rc = ssh_channel_open_session(_channel);
            if (rc != SSH_OK)
                throw SshException(_session->GetSession());
        }

        void Close() const
        {
            auto rc = ssh_channel_close(_channel);
            if (rc != SSH_OK)
                throw SshException(_session->GetSession());
        }

        bool IsOpen() const
        {
            return ssh_channel_is_open(_channel) != 0;
        }

        bool IsClosed() const
        {
            return ssh_channel_is_closed(_channel) != 0;
        }

        bool IsEof() const
        {
            return ssh_channel_is_eof(_channel) != 0;
        }

        void SendEof() const
        {
            auto rc = ssh_channel_send_eof(_channel);
            if (rc != SSH_OK)
                throw SshException(_session->GetSession());
        }

        int GetExitStatus()
        {
            return ssh_channel_get_exit_status(_channel);
        }

        void RequestExec(std::string const& cmd) const
        {
            auto rc = ssh_channel_request_exec(_channel, cmd.c_str());
            if (rc != SSH_OK)
                throw SshException(_session->GetSession());
        }

    private:
        ssh_channel GetChannel() const
        {
            return _channel;
        }

        ssh_channel _channel;
        Session *_session;

    };

    class Connector
    {

    friend class Event;
    
    public:
        Connector() = delete;

        Connector(Session& session) :
            _connector{ssh_connector_new(session.GetSession())}
        {}

        Connector(Connector const&) = delete;
        Connector& operator=(Connector const&) = delete;

        Connector(Connector&&) noexcept = default;
        Connector& operator=(Connector&&) noexcept= default;

        ~Connector()
        {
            ssh_connector_free(_connector);
            _connector = nullptr;
        }

        void SetInFD(socket_t fd) const
        {
            ssh_connector_set_in_fd(_connector, fd);
        }

        void SetOutFD(socket_t fd) const
        {
            ssh_connector_set_out_fd(_connector, fd);
        }

        void SetInChannel(Channel& channel, enum ssh_connector_flags_e flags) const
        {
            ssh_connector_set_in_channel(_connector, channel.GetChannel(), flags);
        }

        void SetOutChannel(Channel& channel, enum ssh_connector_flags_e flags) const
        {
            ssh_connector_set_out_channel(_connector, channel.GetChannel(), flags);
        }

    private:
        ssh_connector GetConnector() const
        {
            return _connector;
        }

        ssh_connector _connector;

    };

    class Event
    {

    public:
        Event() :
            _event{ssh_event_new()}
        {}

        Event(Event const&) = delete;
        Event& operator=(Event const&) = delete;

        Event(Event&&) noexcept = default;
        Event& operator=(Event&&) noexcept = default;

        ~Event()
        {
            ssh_event_free(_event);
            _event = nullptr;
        }

        void DoPoll(int timeout = -1) const
        {
            ssh_event_dopoll(_event, timeout);
        }

        void AddConnector(Connector& connector) const
        {
            ssh_event_add_connector(_event, connector.GetConnector());
        }

        void RemoveConnector(Connector& connector) const
        {
            ssh_event_remove_connector(_event, connector.GetConnector());
        }

    private:
        ssh_event GetEvent() const
        {
            return _event;
        }

        ssh_event _event;

    };

}

#endif
