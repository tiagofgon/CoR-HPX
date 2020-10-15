#ifndef DLL_HPP
#define DLL_HPP

#include <type_traits>
#include <functional>
#include <string>
#include <stdexcept>

#include <dlfcn.h>

namespace dll
{

    class DynamicLoader;
    class DynamicLibrary;

    // http://man7.org/linux/man-pages/man3/dlopen.3.html
    enum class Mode: int
    {
        Lazy        = RTLD_LAZY,
        Now         = RTLD_NOW,
        Local       = RTLD_LOCAL,
        Global      = RTLD_GLOBAL,
        NoLoad      = RTLD_NOLOAD,
        NoDelete    = RTLD_NODELETE
    };

    constexpr typename std::underlying_type<Mode>::type underlying_cast(Mode m) noexcept
    {
        return static_cast<typename std::underlying_type<Mode>::type>(m);
    }

    constexpr Mode operator|(Mode a, Mode b) noexcept
    {
        using underlying = std::underlying_type<Mode>::type;
        return static_cast<Mode>(
            static_cast<underlying>(a)
            | static_cast<underlying>(b)
        );
    }

    class DynamicLibrary
    {

    friend class DynamicLoader;

    public:
        DynamicLibrary() = delete;

        DynamicLibrary(DynamicLibrary const&) = delete;
        DynamicLibrary& operator=(DynamicLibrary const&) = delete;

        DynamicLibrary(DynamicLibrary&&) = delete;
        DynamicLibrary& operator=(DynamicLibrary&&) = delete;

        ~DynamicLibrary()
        {
            dlclose(_handle);
        }

        // load a function from a dynamic library
        template<typename T>
        std::function<T> LoadFunction(std::string const& function) const
        {
            dlerror();
            auto func = dlsym(_handle, function.c_str());
            if (!func) {
                auto error = dlerror();
                if (error)
                    throw std::logic_error("Couldn't find the function \"" + function + "\": " + error);
            }

            return reinterpret_cast<T*>(func);
        }


    
        std::function<void(void)> LoadFunction2(std::string const& function) const
        {
            dlerror();
            typedef void (*hello_t)(void);
            hello_t func = (hello_t)dlsym(_handle, function.c_str());
            if (!func) {
                auto error = dlerror();
                if (error)
                    throw std::logic_error("Couldn't find the function \"" + function + "\": " + error);
            }

            return func;
        }

    protected:
        explicit DynamicLibrary(void *handle) : _handle(handle) {}

    private:
        void *_handle;

    };

    class DynamicLoader
    {

    public:
        // open an dynamic library, in case of success return a DynamicLibrary object
        static DynamicLibrary *LoadDynamicLibrary(std::string const& library, Mode mode = Mode::Lazy)
        {
            void *handle = dlopen(library.c_str(), underlying_cast(mode));
            if (!handle) {
                auto error = dlerror();
                throw std::logic_error("Couldn't load the dynamic library \"" + library + "\": " +  error);
            }

            return new DynamicLibrary(handle);
        }

    };

}

#endif
