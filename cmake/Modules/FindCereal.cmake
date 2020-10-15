set(Cereal_ROOT "${Cereal_ROOT}" CACHE PATH "Cereal root directory")

find_path(Cereal_INCLUDE_DIR
    NAMES
        cereal/cereal.hpp
    PATHS
        "${Cereal_ROOT}"
    PATH_SUFFIXES
        include
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Cereal
    REQUIRED_VARS
        Cereal_INCLUDE_DIR
)

if(Cereal_FOUND)
    set(Cereal_INCLUDE_DIRS ${Cereal_INCLUDE_DIR})
endif()

if(Cereal_FOUND AND NOT TARGET Cereal)
    add_library(Cereal INTERFACE IMPORTED)
    set_target_properties(Cereal PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Cereal_INCLUDE_DIR}")
    target_compile_definitions(Cereal INTERFACE CEREAL_THREAD_SAFE=1)
endif()

mark_as_advanced(Cereal_INCLUDE_DIR)

if(Cereal_FOUND)
    mark_as_advanced(Cereal_ROOT)
endif()
