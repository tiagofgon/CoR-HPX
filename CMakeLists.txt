cmake_minimum_required(VERSION 3.12)

project(PlaCoR LANGUAGES CXX)

#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)


# set(Spread_ROOT /opt/spread-5.0.1)
# set(SSRCSpread_ROOT /opt/libssrcspread-1.0.15)
# set(SSRCSpread_LIBRARY /opt/libssrcspread-1.0.15/lib/libssrcspread.a)
# set(LibEvent_ROOT /opt/libevent-2.1.11)
# set(Cereal_ROOT /opt/cereal-1.3.0)
set(LibSSH_ROOT /opt/libssh-0.8.9)
set(BOOST_ROOT /opt/boost-1.69.0)

# set(LINK_FLAGS /opt/hpx-1.5.1/lib64/libhpx_wrap.a -Wl,-wrap=main)

# find dependencies
# find_package(Spread 5 REQUIRED)
# find_package(SSRCSpread 1.0.15 REQUIRED)
# find_package(LibEvent 2 REQUIRED)
# find_package(LibEventThreads REQUIRED)
# find_package(Cereal REQUIRED)
find_package(HPX REQUIRED)
find_package(LibSSH 0.8 REQUIRED)
find_package(Boost 1.65 REQUIRED COMPONENTS program_options)


# find root and set up environment
#find_package(ROOT CONFIG REQUIRED)
#target_include_directories(ROOT::Core INTERFACE "${ROOT_INCLUDE_DIRS}")

# check subdirectories
add_subdirectory(libcor)
add_subdirectory(tools)
add_subdirectory(units)
add_subdirectory(examples)
add_subdirectory(tests)
#add_subdirectory(cap6)

install(
    DIRECTORY
        "libcor/include/cor"
    DESTINATION
        "include"
    FILES_MATCHING
        PATTERN "*.hpp"
        PATTERN "*.tpp"
        PATTERN "*.h"
)

install(
    DIRECTORY
        "units"
    DESTINATION
        "include/cor"
    FILES_MATCHING
        PATTERN "*.hpp"
        PATTERN "*.tpp"
        PATTERN "*.h"
)
