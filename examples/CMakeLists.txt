
# module
add_library(module SHARED module.cpp)
target_compile_features(module PUBLIC cxx_std_17)
set_target_properties(module PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(module PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(module PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(module PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    module
    PRIVATE
        libcor
	HPX::hpx
)



# # add module_hpx hpx components
# # add_hpx_component(module_hpx
# #     SOURCES module_hpx.cpp
# #     PLUGIN
# #     DEPENDENCIES
# #         libcor
# #         Spread
# #         SSRCSpread
# #         Cereal
# #     COMPILE_FLAGS
# #         -O2
# # )

# basicOperations
add_library(basicOperations SHARED basicOperations.cpp)
target_compile_features(basicOperations PUBLIC cxx_std_17)
set_target_properties(basicOperations PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(basicOperations PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(basicOperations PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(basicOperations PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    basicOperations
    PRIVATE
        libcor
        # Spread
        # SSRCSpread
        # Cereal
	HPX::hpx
)

# callableModule
add_library(callableModule SHARED callableModule.cpp)
target_compile_features(callableModule PUBLIC cxx_std_17)
set_target_properties(callableModule PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(callableModule PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(callableModule PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(callableModule PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    callableModule
    PRIVATE
        libcor
        # Spread
        # SSRCSpread
        # Cereal
	HPX::hpx
)


# parallel
add_library(parallel SHARED parallel.cpp)
target_compile_features(parallel PUBLIC cxx_std_17)
set_target_properties(parallel PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(parallel PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(parallel PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(parallel PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    parallel
    PRIVATE
        libcor
        # Spread
        # SSRCSpread
        # Cereal
	HPX::hpx
)


# parallel2 - HPX
add_library(parallel_hpx2 SHARED parallel_hpx2.cpp)
target_compile_features(parallel_hpx2 PUBLIC cxx_std_17)
set_target_properties(parallel_hpx2 PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(parallel_hpx2 PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(parallel_hpx2 PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(parallel_hpx2 PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    parallel_hpx2
    PRIVATE
        libcor
    PUBLIC
        HPX::hpx
)


# parallel3 - HPX
add_library(parallel_hpx3 SHARED parallel_hpx3.cpp)
target_compile_features(parallel_hpx3 PUBLIC cxx_std_17)
set_target_properties(parallel_hpx3 PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(parallel_hpx3 PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(parallel_hpx3 PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(parallel_hpx3 PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    parallel_hpx3
    PRIVATE
        libcor
    PUBLIC
        HPX::hpx
)


# spawn
add_library(spawn SHARED spawn.cpp)
target_compile_features(spawn PUBLIC cxx_std_17)
set_target_properties(spawn PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(spawn PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(spawn PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(spawn PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    spawn
    PRIVATE
        libcor
        # Spread
        # SSRCSpread
        # Cereal
	HPX::hpx
)

# inter_closure
add_library(inter_closure SHARED inter_closure.cpp)
target_compile_features(inter_closure PUBLIC cxx_std_17)
set_target_properties(inter_closure PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(inter_closure PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(inter_closure PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(inter_closure PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    inter_closure
    PRIVATE
        libcor
        # Spread
        # SSRCSpread
        # Cereal
	HPX::hpx
)
# # spawn - HPX
# add_library(spawn_hpx SHARED spawn_hpx.cpp)
# target_compile_features(spawn_hpx PUBLIC cxx_std_17)
# set_target_properties(spawn_hpx PROPERTIES CXX_EXTENSIONS OFF)
# set_target_properties(spawn_hpx PROPERTIES POSITION_INDEPENDENT_CODE ON)
# set_target_properties(spawn_hpx PROPERTIES COMPILE_FLAGS -O2)
# set_target_properties(spawn_hpx PROPERTIES PREFIX "lib" SUFFIX ".so")
# target_link_libraries(
#     spawn_hpx
#     PRIVATE
#         libcor
#         # Spread
#         # SSRCSpread
#         # Cereal
# 	    HPX::hpx
# )

# # client
# add_library(client SHARED client.cpp)
# target_compile_features(client PUBLIC cxx_std_17)
# set_target_properties(client PROPERTIES CXX_EXTENSIONS OFF)
# set_target_properties(client PROPERTIES POSITION_INDEPENDENT_CODE ON)
# set_target_properties(client PROPERTIES COMPILE_FLAGS -O2)
# set_target_properties(client PROPERTIES PREFIX "lib" SUFFIX ".so")
# target_link_libraries(
#     client
#     PRIVATE
#         libcor
#         # Spread
#         # SSRCSpread
#         # Cereal
# 	HPX::hpx
# )

# # server
# add_library(server SHARED server.cpp)
# target_compile_features(server PUBLIC cxx_std_17)
# set_target_properties(server PROPERTIES CXX_EXTENSIONS OFF)
# set_target_properties(server PROPERTIES POSITION_INDEPENDENT_CODE ON)
# set_target_properties(server PROPERTIES COMPILE_FLAGS -O2)
# set_target_properties(server PROPERTIES PREFIX "lib" SUFFIX ".so")
# target_link_libraries(
#     server
#     PRIVATE
#         libcor
#         # Spread
#         # SSRCSpread
#         # Cereal
# 	hpx
# )

# server_rpc
add_library(server_rpc SHARED server_rpc.cpp)
target_compile_features(server_rpc PUBLIC cxx_std_17)
set_target_properties(server_rpc PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(server_rpc PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(server_rpc PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(server_rpc PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    server_rpc
    PRIVATE
        libcor
        # Spread
        # SSRCSpread
        # Cereal
	HPX::hpx
)

# client_rpc
add_library(client_rpc SHARED client_rpc.cpp)
target_compile_features(client_rpc PUBLIC cxx_std_17)
set_target_properties(client_rpc PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(client_rpc PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(client_rpc PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(client_rpc PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    client_rpc
    PRIVATE
        libcor
        # Spread
        # SSRCSpread
        # Cereal
	HPX::hpx
)

install(
    TARGETS
        module
        basicOperations
        callableModule
        parallel
        parallel_hpx2
        parallel_hpx3
        spawn
        inter_closure
        # spawn_hpx
        # client
        # server
        server_rpc
        client_rpc
    LIBRARY
        DESTINATION examples
)
    