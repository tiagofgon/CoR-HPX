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







	// /*
	// ** Interface for Dynamic Library Loading (DLLoader)
	// ** API for Unix and Windows. Handling of open, close, validity-check.
	// */
	// template <class T>
	// class IDLLoader
	// {

	// public:

	// 	virtual ~IDLLoader() = default;

	// 	/*
	// 	**
	// 	*/
	// 	virtual void DLOpenLib() = 0;

	// 	/*
	// 	** Return a shared pointer on an instance of class loaded through
	// 	** a dynamic library.
	// 	*/
	// 	virtual std::shared_ptr<T>	DLGetInstance() = 0;

	// 	/*
	// 	** Correctly delete the instance of the "dynamically loaded" class.
	// 	*/
	// 	virtual void DLCloseLib() = 0;

	// };



    // template <class T>
	// class DLLoader : public IDLLoader
	// {
	// private:
	// 	void			*_handle;
	// 	std::string		_pathToLib;
	// 	std::string		_allocClassSymbol;
	// 	std::string		_deleteClassSymbol;

	// public:
	// 	DLLoader(std::string const &pathToLib,
	// 		std::string const &allocClassSymbol = "allocator",
	// 		std::string const &deleteClassSymbol = "deleter") :
	// 		  _handle(nullptr), _pathToLib(pathToLib),
	// 		  _allocClassSymbol(allocClassSymbol), 
    //     _deleteClassSymbol(deleteClassSymbol)
	// 	{
	// 	}

	// 	~DLLoader() = default;

	// 	void DLOpenLib()
	// 	{
	// 		if (!(_handle = dlopen(_pathToLib.c_str(), RTLD_NOW | RTLD_LAZY))) {
	// 			std::cerr << dlerror() << std::endl;
	// 		}
	// 	}

	// 	void DLCloseLib() override
	// 	{
	// 		if (dlclose(_handle) != 0) {
	// 			std::cerr << dlerror() << std::endl;
	// 		}
	// 	}

    //     std::shared_ptr<T> DLGetInstance() override
    //     {
    //         using allocClass = T *(*)();
    //         using deleteClass = void (*)(T *);
        
    //         auto allocFunc = reinterpret_cast<T*>( dlsym(_handle, _allocClassSymbol.c_str()));
    //         auto deleteFunc = reinterpret_cast<T*>(
    //                 dlsym(_handle, _deleteClassSymbol.c_str()));
        
    //         if (!allocFunc || !deleteFunc) {
    //             DLCloseLib();
    //             std::cerr << dlerror() << std::endl;
    //         }
        
    //         return std::shared_ptr(
    //                 allocFunc(),
    //                 [deleteFunc](T *p){ deleteFunc(p); });
    //     }

	// };





    // template <class T>
	// class DLLoader : public IDLLoader
	// {
	// private:
	// 	HMODULE			_handle;
	// 	std::string		_pathToLib;
	// 	std::string		_allocClassSymbol;
	// 	std::string		_deleteClassSymbol;
 
	// public:
	// 	DLLoader(std::string const &pathToLib,
	// 		std::string const &allocClassSymbol = "allocator",
	// 		std::string const &deleteClassSymbol = "deleter") :
	// 		_handle(nullptr), _pathToLib(pathToLib),
	// 		_allocClassSymbol(allocClassSymbol), _deleteClassSymbol(deleteClassSymbol)
	// 	{}
 
	// 	~DLLoader() = default;
 
	// 	void DLOpenLib() override
	// 	{
	// 		if (!(_handle = LoadLibrary(_pathToLib.c_str()))) {
	// 			std::cerr << "Can't open and load " << _pathToLib << std::endl;
	// 		}
	// 	}
 
	// 	std::shared_ptr<T> DLGetInstance() override
	// 	{
	// 		using allocClass = T * (*)();
	// 		using deleteClass = void(*)(T *);
 
	// 		auto allocFunc = reinterpret_cast(
	// 			GetProcAddress(_handle, _allocClassSymbol.c_str()));
	// 		auto deleteFunc = reinterpret_cast(
	// 			GetProcAddress(_handle, _deleteClassSymbol.c_str()));
 
	// 		if (!allocFunc || !deleteFunc) {
	// 			DLCloseLib();
	// 			std::cerr << "Can't find allocator or deleter symbol in " << _pathToLib << std::endl;
	// 		}
 
	// 		return std::shared_ptr(
	// 			allocFunc(),
	// 			[deleteFunc](T *p) { deleteFunc(p); });
	// 	}
 
	// 	void DLCloseLib() override
	// 	{
	// 		if (FreeLibrary(_handle) == 0) {
	// 			std::cerr << "Can't close " << _pathToLib << std::endl;
	// 		}
	// 	}
	// };




}

#endif
