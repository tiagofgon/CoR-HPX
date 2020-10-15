#ifndef EVENT_HPP
#define EVENT_HPP

#include <type_traits>
#include <functional>
#include <event.h>

namespace ev
{

	

	namespace thread {

		#include <event2/thread.h>

	}

	class EventBase;
	class Event;

    enum class Events: short
    {
        Timeout 	= EV_TIMEOUT,
        Read 		= EV_READ,
        Write 		= EV_WRITE,
        Signal 		= EV_SIGNAL,
        Persist 	= EV_PERSIST,
        EdgeTrigger = EV_ET
        //Finalize 	= EV_FINALIZE,
        //Closed    = EV_CLOSED
    };

    constexpr typename std::underlying_type<Events>::type underlying_cast(Events ev) noexcept
    {
        return static_cast<typename std::underlying_type<Events>::type>(ev);
    }

    constexpr Events operator|(Events a, Events b) noexcept
    {
        using underlying = std::underlying_type<Events>::type;
        return static_cast<Events>(
            static_cast<underlying>(a)
            | static_cast<underlying>(b)
        );
    }

	class EventBase
	{

	friend class Event;

	public:
		EventBase() : _base(event_base_new()) {}

        EventBase(EventBase const&) = delete;
        EventBase& operator=(EventBase const&) = delete;

        EventBase(EventBase&&) = delete;
        EventBase& operator=(EventBase&&) = delete;

		~EventBase()
		{
			event_base_free(_base);
		}

		inline void loop() const
		{
			event_base_dispatch(_base);
		}

		inline void loop_break() const
		{
			event_base_loopbreak(_base);
		}

	private:
        inline struct event_base *get_event_base() const
		{
			return _base;
		}

		struct event_base *_base;

	};

	class Event
	{

	public:
        Event() = delete;

        Event(Event const&) = delete;
        Event& operator=(Event const&) = delete;

        Event(Event&&) = delete;
        Event& operator=(Event&&) = delete;

		explicit Event(EventBase& base, int fd, Events events, std::function<void()> callback) :
			_ev(nullptr),
			_callback(callback)
		{
			_ev = event_new(base.get_event_base(), fd, underlying_cast(events), 
                        [](int fd, short events, void *arg)
                        {
                            Event *ev = static_cast<Event*>(arg);
                            ev->_callback();
                        }
                , this);
		}

		~Event()
		{
			event_free(_ev);
		}

		inline void start() const
		{
			event_add(_ev, nullptr);
		}

		inline void end() const
		{
			event_del(_ev);
		}

	private:
		struct event *_ev;
		std::function<void()> _callback;

	};

}

#endif
