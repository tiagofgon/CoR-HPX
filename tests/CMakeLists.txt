#[[
# test pool
add_library(test_pool SHARED test_pool.cpp)
target_compile_features(test_pool PUBLIC cxx_std_17)
set_target_properties(test_pool PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(test_pool PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(test_pool PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(test_pool PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    test_pool
    PRIVATE
        libcor
        pool
        Spread
        SSRCSpread
        Cereal
	hpx
)
target_include_directories(
    test_pool
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# test root
add_library(test_root SHARED test_root.cpp)
target_compile_features(test_root PUBLIC cxx_std_17)
set_target_properties(test_root PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(test_root PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(test_root PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(test_root PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    test_root
    PRIVATE
        ROOT::Core
        ${ROOT_LIBRARIES}
        libcor
        pool
        Spread
        SSRCSpread
        Cereal
	hpx
)
target_include_directories(
    test_root
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# vecsum
add_library(vecsum SHARED vecsum.cpp)
target_compile_features(vecsum PUBLIC cxx_std_17)
set_target_properties(vecsum PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(vecsum PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(vecsum PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(vecsum PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    vecsum
    PRIVATE
        libcor
        Spread
        SSRCSpread
        Cereal
	hpx
)

# vecsum_spawn
add_library(vecsum_spawn SHARED vecsum_spawn.cpp)
target_compile_features(vecsum_spawn PUBLIC cxx_std_17)
set_target_properties(vecsum_spawn PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(vecsum_spawn PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(vecsum_spawn PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(vecsum_spawn PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    vecsum_spawn
    PRIVATE
        libcor
        Spread
        SSRCSpread
        Cereal
	hpx
)

# test
add_library(test SHARED test.cpp)
target_compile_features(test PUBLIC cxx_std_17)
set_target_properties(test PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(test PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(test PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(test PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    test
    PRIVATE
        libcor
        Spread
        SSRCSpread
        Cereal
	hpx
)

# dummy
add_library(dummy SHARED dummy.cpp)
target_compile_features(dummy PUBLIC cxx_std_17)
set_target_properties(dummy PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(dummy PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(dummy PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(dummy PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    dummy
    PRIVATE
        ROOT::Core
        ${ROOT_LIBRARIES}
        libcor
        pool
        Spread
        SSRCSpread
        Cereal
	hpx
)
target_include_directories(
    dummy
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# test_clos
add_library(test_clos SHARED test_clos.cpp)
target_compile_features(test_clos PUBLIC cxx_std_17)
set_target_properties(test_clos PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(test_clos PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(test_clos PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(test_clos PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    test_clos
    PRIVATE
        libcor
        pool
        Spread
        SSRCSpread
        Cereal
	hpx
)
target_include_directories(
    test_clos
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# test_client
add_library(test_client SHARED test_client.cpp)
target_compile_features(test_client PUBLIC cxx_std_17)
set_target_properties(test_client PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(test_client PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(test_client PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(test_client PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    test_client
    PRIVATE
        libcor
        pool
        Spread
        SSRCSpread
        Cereal
	hpx
)
target_include_directories(
    test_client
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# test_server
add_library(test_server SHARED test_server.cpp)
target_compile_features(test_server PUBLIC cxx_std_17)
set_target_properties(test_server PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(test_server PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(test_server PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(test_server PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    test_server
    PRIVATE
        libcor
        pool
        Spread
        SSRCSpread
        Cereal
	hpx
)
target_include_directories(
    test_server
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# test_alias
add_library(test_alias SHARED test_alias.cpp)
target_compile_features(test_alias PUBLIC cxx_std_17)
set_target_properties(test_alias PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(test_alias PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(test_alias PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(test_alias PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    test_alias
    PRIVATE
        libcor
        pool
        Spread
        SSRCSpread
        Cereal
	hpx
)
target_include_directories(
    test_alias
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# test_spawn
add_library(test_spawn SHARED test_spawn.cpp)
target_compile_features(test_spawn PUBLIC cxx_std_17)
set_target_properties(test_spawn PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(test_spawn PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(test_spawn PROPERTIES COMPILE_FLAGS -O2)
set_target_properties(test_spawn PROPERTIES PREFIX "lib" SUFFIX ".so")
target_link_libraries(
    test_spawn
    PRIVATE
        libcor
        pool
        Spread
        SSRCSpread
        Cereal
	hpx
)
target_include_directories(
    test_spawn
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
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
        pool
        Spread
        SSRCSpread
        Cereal
	hpx
)
target_include_directories(
    client_rpc
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

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
        pool
        Spread
        SSRCSpread
        Cereal
	hpx
)
target_include_directories(
    server_rpc
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

install(
    TARGETS
        test_pool
        test_root
        vecsum
        vecsum_spawn
        test
        dummy
        test_clos
        test_client
        test_server
        test_alias
        test_spawn
        client_rpc
        server_rpc
    LIBRARY
        DESTINATION tests
)
]]
