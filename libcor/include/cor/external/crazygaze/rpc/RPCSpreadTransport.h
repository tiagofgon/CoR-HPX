#pragma once

#include <ssrc/spread.h>
using namespace ssrc::spread;

#include "cor/external/event/event.hpp"
using namespace ev;

#include "cor/utils/utils.hpp"

namespace cz
{

namespace rpc
{

class BaseSpreadTransport : public Transport, public std::enable_shared_from_this<BaseSpreadTransport>
{

private:
	struct ConstructorCookie {};

public:
	BaseSpreadTransport(ConstructorCookie) {}

	virtual ~BaseSpreadTransport()
	{
		disconnect();
	}

	virtual void send(std::string group, std::vector<char> data) override
	{
		if (m_closed)
			return;

		auto trigger = m_out([&](Out& out)
		{
			if (out.ongoingWrite)
			{
				out.q.push(std::make_pair(std::move(group), std::move(data)));
				return false;
			}
			else
			{
				assert(out.q.size() == 0);
				out.ongoingWrite = true;
				m_outgoing_group = std::move(group);
				m_outgoing_data = std::move(data);
				return true;
			}
		});

		if (trigger) {
			_evwrite->end();
			_evwrite->start();
		}
	}

	virtual bool receive(std::string& group, std::vector<char>& data) override
	{
		if (m_closed)
			return false;

		return m_in([&group, &data](In& in) -> bool
		{
			if (in.q.size() == 0)
			{
				group.clear();
				data.clear();
				return true;
			}
			else
			{
				auto msg = std::move(in.q.front());
				group = std::move(msg.first);
				data = std::move(msg.second);
				in.q.pop();
				return true;
			}
		});
	}

	virtual void close() override
	{
		// mark as closed
		m_closed = true;

		_evread->end();
		_base->loop_break();
	}

	bool connect(std::string const& id)
	{
        std::string name = "R" + id;

	    // instanciate communication system
	    _mbox = new ssrc::spread::Mailbox("4801", name, false, ssrc::spread::Mailbox::High);
	    _base = new EventBase();
	    _evread = new Event(*_base, _mbox->descriptor(), Events::Read | Events::Persist, [this](){ HandleRead(); });
	    _evwrite = new Event(*_base, _mbox->descriptor(), Events::Write | Events::Persist, [this](){ HandleWrite(); });

		// create service thread
    	_th_svc = std::move(std::thread(&BaseSpreadTransport::operator(), this));

		_in_smsg.add(_in_msg);

    	return true;
	}

	void operator()()
	{
		_evread->start();
    	_base->loop();
	}

	void disconnect()
	{
		close();
		_th_svc.join();
	}

protected:
	template <typename LOCAL, typename REMOTE>
	static std::future<std::shared_ptr<Connection<LOCAL, REMOTE>>>
		create_impl(LOCAL* local_obj, std::string const& id)
	{
		auto trp = std::make_shared<BaseSpreadTransport>(ConstructorCookie());
		auto pr = std::make_shared<std::promise<std::shared_ptr<Connection<LOCAL, REMOTE>>>>();

		if (trp->connect(id)) {
			auto con = std::make_shared<Connection<LOCAL, REMOTE>>(local_obj, trp);
			trp->m_con = con.get();
			pr->set_value(std::move(con));
		} else {
			pr->set_value(nullptr);
		}

		return pr->get_future();
	}

	void HandleRead()
	{
		// receive message
		_mbox->receive(_in_smsg);

		m_incoming_group = _in_smsg.sender();
		m_incoming_data.assign(_in_msg.begin(), _in_msg.end());

		// clear message content
		_in_msg.clear();

		m_in([this](In& in)
		{
			in.q.push(std::make_pair(std::move(m_incoming_group), std::move(m_incoming_data)));
		});
		m_con->process();
	}

	void HandleWrite()
	{
		// build message
		_out_smsg.set_safe();
		_out_smsg.set_type(0);
		_out_smsg.add(m_outgoing_data.data(), m_outgoing_data.size());
		_out_groups.add(m_outgoing_group);

		// send message
		_mbox->send(_out_smsg, _out_groups);

		// clear message content
		_out_smsg.clear();
		_out_groups.clear();

		m_out([&](Out& out)
		{
			if (out.q.size())
			{
				auto msg = std::move(out.q.front());
				m_outgoing_group = std::move(msg.first);
				m_outgoing_data = std::move(msg.second);
				out.q.pop();
			}
			else
			{
				out.ongoingWrite = false;
				m_outgoing_group.clear();
				m_outgoing_data.clear();
			}
		});

		_evwrite->end();

		if (m_outgoing_data.size())
			_evwrite->start();
	}

	std::thread _th_svc;

	// communication system
    Mailbox *_mbox;
    EventBase *_base;
    Event *_evread;
    Event *_evwrite;

    // variables to handle received messages
    ScatterMessage _in_smsg;
    Message _in_msg;

    GroupList _out_groups;
    ScatterMessage _out_smsg;

	bool m_closeStarted = false;
	bool m_closed = false;
	BaseConnection* m_con;
	std::function<void()> m_onClosed;

	struct Out
	{
		bool ongoingWrite = false;
		std::queue<std::pair<std::string, std::vector<char>>> q;
	};
	Monitor<Out> m_out;

	struct In
	{
		std::queue<std::pair<std::string, std::vector<char>>> q;
	};
	Monitor<In> m_in;

	// Holds the next incoming RPC data
	std::string m_incoming_group;
	std::vector<char> m_incoming_data;

	// Hold the currently outgoing RPC data
	std::string m_outgoing_group;
	std::vector<char> m_outgoing_data;

};

template <typename LOCAL, typename REMOTE>
class SpreadTransport : public BaseSpreadTransport
{
public:
	static std::future<std::shared_ptr<Connection<LOCAL, REMOTE>>>
		create(LOCAL& local_obj, std::string const& id)
	{
		return create_impl<LOCAL, REMOTE>(&local_obj, id);
	}
};

}
}
