#ifndef SP_EVENTS_HPP
#define SP_EVENTS_HPP

#include <type_traits>
#include <functional>

namespace spread
{
    namespace events
    {

        namespace internal
        {
            #include <sp_events.h>
        }

        enum class Priority: short
        {
            Low     = LOW_PRIORITY,
            Medium  = MEDIUM_PRIORITY,
            High    = HIGH_PRIORITY
        };

        constexpr typename std::underlying_type<Priority>::type underlying_cast(Priority p) noexcept
        {
            return static_cast<typename ::std::underlying_type<Priority>::type>(p);
        }

        enum class FdType: short
        {
            Read    = READ_FD,
            Write   = WRITE_FD,
            Except  = EXCEPT_FD
        };

        constexpr typename std::underlying_type<FdType>::type underlying_cast(FdType t) noexcept
        {
            return static_cast<typename ::std::underlying_type<FdType>::type>(t);
        }

        constexpr FdType operator|(FdType a, FdType b) noexcept
        {
            using underlying = std::underlying_type<FdType>::type;
            return static_cast<FdType>(
                static_cast<underlying>(a)
                | static_cast<underlying>(b)
            );
        }

        class EventsHandler
        {

        public:
            EventsHandler()
            {
                internal::E_init();
            }

            ~EventsHandler() = default;

            void AttachFd(int fd, FdType fd_type, void (*callback) (int fd, int code, void *data), int code, void *data, Priority priority)
            {
                internal::E_attach_fd(fd, underlying_cast(fd_type), callback, code, data, underlying_cast(priority));
            }

            void DetachFd(int fd, FdType fd_type)
            {
                internal::E_detach_fd(fd, underlying_cast(fd_type));
            }

            void DetachFdPriority(int fd, FdType fd_type, Priority priority)
            {
                internal::E_detach_fd_priority(fd, underlying_cast(fd_type), underlying_cast(priority));
            }

            void SetActiveThreshold(Priority priority)
            {
                internal::E_set_active_threshold(underlying_cast(priority));
            }

            void ActivateFd(int fd, FdType fd_type)
            {
                internal::E_activate_fd(fd, underlying_cast(fd_type));
            }

            void DeactivateFd(int fd, FdType fd_type)
            {
                internal::E_deactivate_fd(fd, underlying_cast(fd_type));
            }

            void NumActive(Priority priority)
            {
                internal::E_num_active(underlying_cast(priority));
            }

            void HandleEvents()
            {
                internal::E_handle_events();
            }

            void ExitEvents()
            {
                internal::E_exit_events_async_safe();
            }

        };

    }
}


#endif
