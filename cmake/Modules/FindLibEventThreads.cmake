get_filename_component(LibEvent_MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(LibEvent_ROOT "${LibEvent_ROOT}" CACHE PATH "LibEvent root directory")

find_path(LibEventThreads_INCLUDE_DIR
    NAMES
        event2/event.h        
    PATHS
        "${LibEvent_ROOT}"
    PATH_SUFFIXES
        include
)

set(LibEventThreads_LIBRARY_NAME "libevent_pthreads${CMAKE_SHARED_LIBRARY_SUFFIX}")
if(LibEvent_USE_STATIC)
    set(LibEventThreads_LIBRARY_NAME "libevent_pthreads${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

find_library(LibEventThreads_LIBRARY
    NAMES
        ${LibEventThreads_LIBRARY_NAME}
    PATHS
        "${LibEvent_ROOT}"
    PATH_SUFFIXES
        lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibEventThreads
    REQUIRED_VARS
        LibEventThreads_LIBRARY
        LibEventThreads_INCLUDE_DIR
)

if(LibEventThreads_FOUND)
    set(LibEventThreads_INCLUDE_DIRS ${LibEventThreads_INCLUDE_DIR})
    set(LibEventThreads_LIBRARIES ${LibEventThreads_LIBRARY})
endif()

if(LibEventThreads_FOUND AND NOT TARGET LibEventThreads)
    add_library(LibEventThreads UNKNOWN IMPORTED)
    set_target_properties(LibEventThreads PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${LibEventThreads_INCLUDE_DIR})
    set_target_properties(LibEventThreads PROPERTIES IMPORTED_LOCATION ${LibEventThreads_LIBRARY})
endif()

mark_as_advanced(LibEventThreads_INCLUDE_DIR LibEventThreads_LIBRARY_NAME LibEventThreads_LIBRARY)

if(LibEventThreads_FOUND)
    mark_as_advanced(LibEvent_ROOT)
endif()
