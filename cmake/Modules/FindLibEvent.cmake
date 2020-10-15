get_filename_component(LibEvent_MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(LibEvent_ROOT "${LibEvent_ROOT}" CACHE PATH "LibEvent root directory")

find_path(LibEvent_INCLUDE_DIR
    NAMES
        event.h        
    PATHS
        "${LibEvent_ROOT}"
    PATH_SUFFIXES
        include
)

set(LibEvent_LIBRARY_NAME "libevent${CMAKE_SHARED_LIBRARY_SUFFIX}")
if(LibEvent_USE_STATIC)
    set(LibEvent_LIBRARY_NAME "libevent${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

find_library(LibEvent_LIBRARY
    NAMES
        ${LibEvent_LIBRARY_NAME}
    PATHS
        "${LibEvent_ROOT}"
    PATH_SUFFIXES
        lib
)

if(NOT LibEvent_FIND_VERSION)
    set(LibEvent_VERSION_ACCEPTABLE TRUE CACHE BOOL "LibEvent version check was not required" FORCE)
endif()

if(LibEvent_FIND_VERSION AND NOT LibEvent_VERSION_ACCEPTABLE)

    try_run(LibEvent_VERSION_RUN_RESULT LibEvent_VERSION_COMPILE_RESULT
        "${CMAKE_BINARY_DIR}"
        "${LibEvent_MODULE_DIR}/ExtractLibEventVersion.cpp"
        CMAKE_FLAGS -DINCLUDE_DIRECTORIES:STRING=${LibEvent_INCLUDE_DIR}
        RUN_OUTPUT_VARIABLE LibEvent_VERSION
        COMPILE_OUTPUT_VARIABLE LibEvent_VERSION_COMPILE_OUT
    )
    
    if(NOT LibEvent_VERSION_COMPILE_RESULT)
        message(STATUS "Unable to determine LibEvent version because the compilation of the test program failed")
        set(LibEvent_VERSION_ACCEPTABLE FALSE)
    elseif(NOT LibEvent_VERSION_RUN_RESULT EQUAL 0)
        message(STATUS "Unable to determine LibEvent version because the execution of the test program failed")
        set(LibEvent_VERSION_ACCEPTABLE FALSE)
    elseif(${LibEvent_VERSION} VERSION_LESS ${LibEvent_FIND_VERSION})
        message(STATUS "Requested LibEvent version ${LibEvent_VERSION} does not satisfy requirement ${LibEvent_FIND_VERSION}")
        set(LibEvent_VERSION_ACCEPTABLE FALSE)
    else()
        set(LibEvent_VERSION_ACCEPTABLE TRUE CACHE BOOL "LibEvent version was checked and is sufficient" FORCE)
    endif()
    
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibEvent
    REQUIRED_VARS
        LibEvent_LIBRARY
        LibEvent_INCLUDE_DIR
        LibEvent_VERSION_ACCEPTABLE
)

if(LibEvent_FOUND)
    set(LibEvent_INCLUDE_DIRS ${LibEvent_INCLUDE_DIR})
    set(LibEvent_LIBRARIES ${LibEvent_LIBRARY})
endif()

if(LibEvent_FOUND AND NOT TARGET LibEvent)
    add_library(LibEvent UNKNOWN IMPORTED)
    set_target_properties(LibEvent PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${LibEvent_INCLUDE_DIR})
    set_target_properties(LibEvent PROPERTIES IMPORTED_LOCATION ${LibEvent_LIBRARY})
endif()

mark_as_advanced(LibEvent_INCLUDE_DIR LibEvent_LIBRARY_NAME LibEvent_LIBRARY LibEvent_VERSION_ACCEPTABLE)

if(LibEvent_FOUND)
    mark_as_advanced(LibEvent_ROOT)
endif()
