cmake_minimum_required(VERSION 3.12)

project(PlaCoR LANGUAGES CXX)

#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

# set(LibSSH_ROOT /opt/libssh-0.8.9)

# find dependencies
find_package(HPX REQUIRED)
find_package(LibSSH 0.8 REQUIRED)

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
