get_filename_component(SSRCSpread_MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(SSRCSpread_ROOT "${SSRCSpread_ROOT}" CACHE PATH "SSRCSpread root directory")

find_path(SSRCSpread_INCLUDE_DIR
    NAMES
        ssrc/spread.h       
    PATHS
        "${SSRCSpread_ROOT}"
    PATH_SUFFIXES
        include
)

set(SSRCSpread_LIBRARY_NAME "libssrcspread${CMAKE_SHARED_LIBRARY_SUFFIX}")
if(SSRCSpread_USE_STATIC)
    set(SSRCSpread_LIBRARY_NAME "libssrcspread${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

find_library(SSRCSpread_LIBRARY
    NAMES
        ${SSRCSpread_LIBRARY_NAME}
    PATHS
        "${SSRCSpread_ROOT}"
    PATH_SUFFIXES
        lib
)

if(NOT SSRCSpread_FIND_VERSION)
    set(SSRCSpread_VERSION_ACCEPTABLE TRUE CACHE BOOL "SSRCSpread version check was not required" FORCE)
endif()

if(SSRCSpread_FIND_VERSION AND NOT SSRCSpread_VERSION_ACCEPTABLE)

    try_run(SSRCSpread_VERSION_RUN_RESULT SSRCSpread_VERSION_COMPILE_RESULT
        "${CMAKE_BINARY_DIR}"
        "${SSRCSpread_MODULE_DIR}/ExtractSSRCSpreadVersion.cpp"
        CMAKE_FLAGS -DINCLUDE_DIRECTORIES:STRING=${SSRCSpread_INCLUDE_DIR}
        RUN_OUTPUT_VARIABLE SSRCSpread_VERSION
        COMPILE_OUTPUT_VARIABLE SSRCSpread_VERSION_COMPILE_OUT
    )
    
    if(NOT SSRCSpread_VERSION_COMPILE_RESULT)
        message(STATUS "Unable to determine SSRCSpread version because the compilation of the test program failed")
        set(SSRCSpread_VERSION_ACCEPTABLE FALSE)
    elseif(NOT SSRCSpread_VERSION_RUN_RESULT EQUAL 0)
        message(STATUS "Unable to determine SSRCSpread version because the execution of the test program failed")
        set(SSRCSpread_VERSION_ACCEPTABLE FALSE)
    elseif(${SSRCSpread_VERSION} VERSION_LESS ${SSRCSpread_FIND_VERSION})
        message(STATUS "Requested SSRCSpread version ${SSRCSpread_VERSION} does not satisfy requirement ${SSRCSpread_FIND_VERSION}")
        set(SSRCSpread_VERSION_ACCEPTABLE FALSE)
    else()
        set(SSRCSpread_VERSION_ACCEPTABLE TRUE CACHE BOOL "SSRCSpread version was checked and is sufficient" FORCE)
    endif()
    
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SSRCSpread
    REQUIRED_VARS
        SSRCSpread_LIBRARY
        SSRCSpread_INCLUDE_DIR
        SSRCSpread_VERSION_ACCEPTABLE
)

if(SSRCSpread_FOUND)
    set(SSRCSpread_INCLUDE_DIRS ${SSRCSpread_INCLUDE_DIR})
    set(SSRCSpread_LIBRARIES ${SSRCSpread_LIBRARY})
endif()

if(SSRCSpread_FOUND AND NOT TARGET SSRCSpread)
    add_library(SSRCSpread UNKNOWN IMPORTED)
    set_target_properties(SSRCSpread PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SSRCSpread_INCLUDE_DIR})
    set_target_properties(SSRCSpread PROPERTIES IMPORTED_LOCATION ${SSRCSpread_LIBRARY})
endif()

mark_as_advanced(SSRCSpread_INCLUDE_DIR SSRCSpread_LIBRARY_NAME SSRCSpread_LIBRARY SSRCSpread_VERSION_ACCEPTABLE)

if(SSRCSpread_FOUND)
    mark_as_advanced(SSRCSpread_ROOT)
endif()
