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

)
target_include_directories(
    mcpi
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
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
#         pool
#         Spread
#         SSRCSpread
#         Cereal
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
#         pool
#         Spread
#         SSRCSpread
#         Cereal
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
#         pool
#         Spread
#         SSRCSpread
#         Cereal
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
#         pool
#         Spread
#         SSRCSpread
#         Cereal
# )
# target_include_directories(
#     read_files_dist_v3
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
#         $<INSTALL_INTERFACE:include>
# )

# # inter_closure
# add_library(inter_closure SHARED inter_closure.cpp)
# target_compile_features(inter_closure PUBLIC cxx_std_17)
# set_target_properties(inter_closure PROPERTIES CXX_EXTENSIONS OFF)
# set_target_properties(inter_closure PROPERTIES POSITION_INDEPENDENT_CODE ON) 
# set_target_properties(inter_closure PROPERTIES COMPILE_FLAGS -O2) 
# set_target_properties(inter_closure PROPERTIES PREFIX "lib" SUFFIX ".so")
# target_link_libraries(
# 	inter_closure
#     PRIVATE
# 	    ROOT::Core
#         ${ROOT_LIBRARIES}
#         libcor
#         pool
#         Spread
#         SSRCSpread
#         Cereal
# )
# target_include_directories(
#     inter_closure
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
#         $<INSTALL_INTERFACE:include>
# )

# # client_rpc
# add_library(client_rpc SHARED client_rpc.cpp)
# target_compile_features(client_rpc PUBLIC cxx_std_17)
# set_target_properties(client_rpc PROPERTIES CXX_EXTENSIONS OFF)
# set_target_properties(client_rpc PROPERTIES POSITION_INDEPENDENT_CODE ON) 
# set_target_properties(client_rpc PROPERTIES COMPILE_FLAGS -O2) 
# set_target_properties(client_rpc PROPERTIES PREFIX "lib" SUFFIX ".so")
# target_link_libraries(
# 	client_rpc
#     PRIVATE
#         libcor
#         pool
#         Spread
#         SSRCSpread
#         Cereal
# )
# target_include_directories(
#     client_rpc
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
#         $<INSTALL_INTERFACE:include>
# )

# # server_rpc
# add_library(server_rpc SHARED server_rpc.cpp)
# target_compile_features(server_rpc PUBLIC cxx_std_17)
# set_target_properties(server_rpc PROPERTIES CXX_EXTENSIONS OFF)
# set_target_properties(server_rpc PROPERTIES POSITION_INDEPENDENT_CODE ON) 
# set_target_properties(server_rpc PROPERTIES COMPILE_FLAGS -O2) 
# set_target_properties(server_rpc PROPERTIES PREFIX "lib" SUFFIX ".so")
# target_link_libraries(
# 	server_rpc
#     PRIVATE
#         libcor
#         pool
#         Spread
#         SSRCSpread
#         Cereal
# )
# target_include_directories(
#     server_rpc
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
#         $<INSTALL_INTERFACE:include>
# )

install(
    TARGETS
        mcpi
        # read_files_par
        # read_files_dist_v1
        # read_files_dist_v2
        # read_files_dist_v3
        # inter_closure
        # client_rpc
        # server_rpc
    LIBRARY
        DESTINATION cap6
)
