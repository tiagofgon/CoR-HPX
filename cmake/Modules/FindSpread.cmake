get_filename_component(Spread_MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(Spread_ROOT "${Spread_ROOT}" CACHE PATH "Spread root directory")

find_path(Spread_INCLUDE_DIR
    NAMES
        sp.h
    PATHS
        "${Spread_ROOT}"
    PATH_SUFFIXES
        include
)

find_program(Spread_EXECUTABLE
    NAMES
        spread
    PATHS
        "${Spread_ROOT}"
    PATH_SUFFIXES
        bin
        sbin
)

set(Spread_LIBRARY_NAME "libtspread-core${CMAKE_SHARED_LIBRARY_SUFFIX}")
if(Spread_USE_STATIC)
    set(Spread_LIBRARY_NAME "libtspread-core${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

find_library(Spread_LIBRARY
    NAMES
        ${Spread_LIBRARY_NAME}
    PATHS
        "${Spread_ROOT}"
    PATH_SUFFIXES
        lib
)

if(NOT Spread_FIND_VERSION)
    set(Spread_VERSION_ACCEPTABLE TRUE CACHE BOOL "Spread version check was not required" FORCE)
endif()

if(Spread_FIND_VERSION AND NOT Spread_VERSION_ACCEPTABLE)

    try_run(Spread_VERSION_RUN_RESULT Spread_VERSION_COMPILE_RESULT
        "${CMAKE_BINARY_DIR}"
        "${Spread_MODULE_DIR}/ExtractSpreadVersion.cpp"
        CMAKE_FLAGS -DINCLUDE_DIRECTORIES:STRING=${Spread_INCLUDE_DIR}
        RUN_OUTPUT_VARIABLE Spread_VERSION
        COMPILE_OUTPUT_VARIABLE Spread_VERSION_COMPILE_OUT
    )

    if(NOT Spread_VERSION_COMPILE_RESULT)
        message(STATUS "Unable to determine spread version because the compilation of the test program failed")
        set(Spread_VERSION_ACCEPTABLE FALSE)
    elseif(NOT Spread_VERSION_RUN_RESULT EQUAL 0)
        message(STATUS "Unable to determine spread version because the execution of the test program failed")
        set(Spread_VERSION_ACCEPTABLE FALSE)
    elseif(${Spread_VERSION} VERSION_LESS ${Spread_FIND_VERSION})
        message(STATUS "Requested spread version ${Spread_VERSION} does not satisfy requirement ${Spread_FIND_VERSION}")
        set(Spread_VERSION_ACCEPTABLE FALSE)
    else()
        set(Spread_VERSION_ACCEPTABLE TRUE CACHE BOOL "Spread version was checked and is sufficient" FORCE)
    endif()

endif()

include(FindPackageHandleStandardArgs)
if(Spread_IGNORE_EXECUTABLE)
    find_package_handle_standard_args(Spread
        REQUIRED_VARS
            Spread_LIBRARY
            Spread_INCLUDE_DIR
            Spread_VERSION_ACCEPTABLE
    )
else()
    find_package_handle_standard_args(Spread
        REQUIRED_VARS
            Spread_LIBRARY
            Spread_INCLUDE_DIR
            Spread_EXECUTABLE
            Spread_VERSION_ACCEPTABLE
        )
endif()

if(Spread_FOUND)
    set(Spread_INCLUDE_DIRS ${Spread_INCLUDE_DIR})
    set(Spread_LIBRARIES ${Spread_LIBRARY})
endif()

if(Spread_FOUND AND NOT TARGET Spread)
    add_library(Spread UNKNOWN IMPORTED)
    set_target_properties(Spread PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${Spread_INCLUDE_DIR})
    set_target_properties(Spread PROPERTIES IMPORTED_LOCATION ${Spread_LIBRARY})
endif()

mark_as_advanced(Spread_INCLUDE_DIR Spread_LIBRARY_NAME Spread_LIBRARY Spread_EXECUTABLE Spread_VERSION_ACCEPTABLE)

if(Spread_FOUND)
    mark_as_advanced(Spread_ROOT)
endif()
