get_filename_component(LibSSH_MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(LibSSH_ROOT "${LibSSH_ROOT}" CACHE PATH "LibSSH root directory")

find_path(LibSSH_INCLUDE_DIR
    NAMES
        libssh/libssh.h      
    PATHS
        "${LibSSH_ROOT}"
    PATH_SUFFIXES
        include
)

set(LibSSH_LIBRARY_NAME "libssh${CMAKE_SHARED_LIBRARY_SUFFIX}")
if(LibSSH_USE_STATIC)
    set(LibSSH_LIBRARY_NAME "libssh${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

find_library(LibSSH_LIBRARY
    NAMES
        ${LibSSH_LIBRARY_NAME}
    PATHS
        "${LibSSH_ROOT}"
    PATH_SUFFIXES
        lib
)

if(NOT LibSSH_FIND_VERSION)
    set(LibSSH_VERSION_ACCEPTABLE TRUE CACHE BOOL "LibSSH version check was not required" FORCE)
endif()

if(LibSSH_FIND_VERSION AND NOT LibSSH_VERSION_ACCEPTABLE)

    try_run(LibSSH_VERSION_RUN_RESULT LibSSH_VERSION_COMPILE_RESULT
        "${CMAKE_BINARY_DIR}"
        "${LibSSH_MODULE_DIR}/ExtractLibSSHVersion.cpp"
        CMAKE_FLAGS -DINCLUDE_DIRECTORIES:STRING=${LibSSH_INCLUDE_DIR}
        RUN_OUTPUT_VARIABLE LibSSH_VERSION
        COMPILE_OUTPUT_VARIABLE LibSSH_VERSION_COMPILE_OUT
    )
    
    if(NOT LibSSH_VERSION_COMPILE_RESULT)
        message(STATUS "Unable to determine LibSSH version because the compilation of the test program failed")
        set(LibSSH_VERSION_ACCEPTABLE FALSE)
    elseif(NOT LibSSH_VERSION_RUN_RESULT EQUAL 0)
        message(STATUS "Unable to determine LibSSH version because the execution of the test program failed")
        set(LibSSH_VERSION_ACCEPTABLE FALSE)
    elseif(${LibSSH_VERSION} VERSION_LESS ${LibSSH_FIND_VERSION})
        message(STATUS "Requested LibSSH version ${LibSSH_VERSION} does not satisfy requirement ${LibSSH_FIND_VERSION}")
        set(LibSSH_VERSION_ACCEPTABLE FALSE)
    else()
        set(LibSSH_VERSION_ACCEPTABLE TRUE CACHE BOOL "LibSSH version was checked and is sufficient" FORCE)
    endif()
    
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibSSH
    REQUIRED_VARS
        LibSSH_LIBRARY
        LibSSH_INCLUDE_DIR
        LibSSH_VERSION_ACCEPTABLE
)

if(LibSSH_FOUND)
    set(LibSSH_INCLUDE_DIRS ${LibSSH_INCLUDE_DIR})
    set(LibSSH_LIBRARIES ${LibSSH_LIBRARY})
endif()

if(LibSSH_FOUND AND NOT TARGET LibSSH)
    add_library(LibSSH UNKNOWN IMPORTED)
    set_target_properties(LibSSH PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${LibSSH_INCLUDE_DIR})
    set_target_properties(LibSSH PROPERTIES IMPORTED_LOCATION ${LibSSH_LIBRARY})
endif()

mark_as_advanced(LibSSH_INCLUDE_DIR LibSSH_LIBRARY_NAME LibSSH_LIBRARY LibSSH_VERSION_ACCEPTABLE)

if(LibSSH_FOUND)
    mark_as_advanced(LibSSH_ROOT)
endif()
