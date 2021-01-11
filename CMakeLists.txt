cmake_minimum_required(VERSION 3.0)

project(PlaCoR LANGUAGES CXX VERSION 1.0)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/libcor/include/cor/PlaCoRConfig.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/libcor/include/cor/PlaCoRConfig.h" )


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

#set(LibSSH_ROOT /share/apps/libssh-0.8.6)
#set(LibSSH_ROOT /share/apps/root_v6.16)

# find dependencies
find_package(HPX REQUIRED)
find_package(LibSSH REQUIRED)

# find root and set up environment
# find_package(ROOT CONFIG REQUIRED)
# target_include_directories(ROOT::Core INTERFACE "${ROOT_INCLUDE_DIRS}")

# check subdirectories
add_subdirectory(libcor)
add_subdirectory(tools)
add_subdirectory(examples)
add_subdirectory(tests)
add_subdirectory(cap6)

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




set(DEF_INSTALL_CMAKE_DIR CMake)
set(DEF_INSTALL_CMAKE_DIR lib/CMake/PlaCor)

set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# set up include-directories
include_directories(
  "${PROJECT_SOURCE_DIR}"   # to find foo/foo.h
  "${PROJECT_BINARY_DIR}")  # to find foo/config.h

# The interesting stuff goes here
# ===============================

# Add all targets to the build-tree export set
export(TARGETS libcor FILE "${PROJECT_BINARY_DIR}/PlaCoRTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE PlaCoR)

# Create the PlaCoRConfig.cmake and PlaCoRConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(PlaCoRConfig.cmake.in
  "${PROJECT_BINARY_DIR}/PlaCoRConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${PLACOR_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(PlaCoRConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/PlaCoRConfig.cmake" @ONLY)
# ... for both
configure_file(PlaCoRConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/PlaCoRConfigVersion.cmake" @ONLY)

# Install the PlaCoRConfig.cmake and PlaCoRConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/PlaCoRConfig.cmake"
  "${PROJECT_BINARY_DIR}/PlaCoRConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
#install(EXPORT PlaCoRTargets DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
