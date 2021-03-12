# mcpi
add_library(mcpi SHARED mcpi.cpp)
target_compile_features(mcpi PUBLIC cxx_std_17)
set_target_properties(mcpi PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(mcpi PROPERTIES POSITION_INDEPENDENT_CODE ON) 
set_target_properties(mcpi PROPERTIES COMPILE_FLAGS -O2) 
set_target_properties(mcpi PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
	mcpi
    PRIVATE
        libcor
        HPX::hpx
	    # ROOT::Core
        # ${ROOT_LIBRARIES}

)
target_include_directories(
    mcpi
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# mcpi_for_loop
add_library(mcpi_for_loop SHARED mcpi_for_loop.cpp)
target_compile_features(mcpi_for_loop PUBLIC cxx_std_17)
set_target_properties(mcpi_for_loop PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(mcpi_for_loop PROPERTIES POSITION_INDEPENDENT_CODE ON) 
set_target_properties(mcpi_for_loop PROPERTIES COMPILE_FLAGS -O2) 
set_target_properties(mcpi_for_loop PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
	mcpi_for_loop
    PRIVATE
        libcor
        HPX::hpx

)
target_include_directories(
    mcpi_for_loop
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# friendly_numbers
add_library(friendly_numbers SHARED friendly_numbers.cpp)
target_compile_features(friendly_numbers PUBLIC cxx_std_17)
set_target_properties(friendly_numbers PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(friendly_numbers PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(friendly_numbers PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(friendly_numbers PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    friendly_numbers
    PRIVATE
        libcor
	    HPX::hpx
)

# # read_files_par
# add_library(read_files_par SHARED read_files_par.cpp)
# target_compile_features(read_files_par PUBLIC cxx_std_17)
# set_target_properties(read_files_par PROPERTIES CXX_EXTENSIONS OFF)
# set_target_properties(read_files_par PROPERTIES POSITION_INDEPENDENT_CODE ON) 
# set_target_properties(read_files_par PROPERTIES COMPILE_FLAGS -O2) 
# set_target_properties(read_files_par PROPERTIES PREFIX "lib" SUFFIX ".so")
# target_link_libraries(
# 	read_files_par
#     PRIVATE
# 	    ROOT::Core
#         ${ROOT_LIBRARIES}
#         libcor
# )
# target_include_directories(
#     read_files_par
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
#         $<INSTALL_INTERFACE:include>
# )

# # read_files_dist_v1
# add_library(read_files_dist_v1 SHARED read_files_dist_v1.cpp)
# target_compile_features(read_files_dist_v1 PUBLIC cxx_std_17)
# set_target_properties(read_files_dist_v1 PROPERTIES CXX_EXTENSIONS OFF)
# set_target_properties(read_files_dist_v1 PROPERTIES POSITION_INDEPENDENT_CODE ON) 
# set_target_properties(read_files_dist_v1 PROPERTIES COMPILE_FLAGS -O2) 
# set_target_properties(read_files_dist_v1 PROPERTIES PREFIX "lib" SUFFIX ".so")
# target_link_libraries(
# 	read_files_dist_v1
#     PRIVATE
# 	    ROOT::Core
#         ${ROOT_LIBRARIES}
#         libcor
# )
# target_include_directories(
#     read_files_dist_v1
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
#         $<INSTALL_INTERFACE:include>
# )

# # read_files_dist_v2
# add_library(read_files_dist_v2 SHARED read_files_dist_v2.cpp)
# target_compile_features(read_files_dist_v2 PUBLIC cxx_std_17)
# set_target_properties(read_files_dist_v2 PROPERTIES CXX_EXTENSIONS OFF)
# set_target_properties(read_files_dist_v2 PROPERTIES POSITION_INDEPENDENT_CODE ON) 
# set_target_properties(read_files_dist_v2 PROPERTIES COMPILE_FLAGS -O2) 
# set_target_properties(read_files_dist_v2 PROPERTIES PREFIX "lib" SUFFIX ".so")
# target_link_libraries(
# 	read_files_dist_v2
#     PRIVATE
# 	    ROOT::Core
#         ${ROOT_LIBRARIES}
#         libcor
# )
# target_include_directories(
#     read_files_dist_v2
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
#         $<INSTALL_INTERFACE:include>
# )

# # read_files_dist_v3
# add_library(read_files_dist_v3 SHARED read_files_dist_v3.cpp)
# target_compile_features(read_files_dist_v3 PUBLIC cxx_std_17)
# set_target_properties(read_files_dist_v3 PROPERTIES CXX_EXTENSIONS OFF)
# set_target_properties(read_files_dist_v3 PROPERTIES POSITION_INDEPENDENT_CODE ON) 
# set_target_properties(read_files_dist_v3 PROPERTIES COMPILE_FLAGS -O2) 
# set_target_properties(read_files_dist_v3 PROPERTIES PREFIX "lib" SUFFIX ".so")
# target_link_libraries(
# 	read_files_dist_v3
#     PRIVATE
# 	    ROOT::Core
#         ${ROOT_LIBRARIES}
#         libcor
# )
# target_include_directories(
#     read_files_dist_v3
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
#         $<INSTALL_INTERFACE:include>
# )



install(
    TARGETS
        mcpi
        mcpi_for_loop
        friendly_numbers
        # read_files_par
        # read_files_dist_v1
        # read_files_dist_v2
        # read_files_dist_v3
    LIBRARY
        DESTINATION cap6
)
