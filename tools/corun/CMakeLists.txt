# add_executable(corun corun.cpp host_file_parser.cpp)

# # add_hpx_executable(corun 
# # 	SOURCES corun.cpp host_file_parser.cpp
# # 	DEPENDENCIES 
# #         LibSSH
# #         Boost::program_options
# #         libcor
# # )

# target_compile_features(corun PUBLIC cxx_std_17)
# set_target_properties(corun PROPERTIES CXX_EXTENSIONS OFF)
# set_target_properties(corun PROPERTIES COMPILE_FLAGS -O2)
# target_include_directories(
#     corun
#     PRIVATE
#         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libcor/include>
# )

# install(
#     TARGETS
#         corun
#     RUNTIME
#         DESTINATION bin
# )

add_executable(corun corun.cpp host_file_parser.cpp)

target_compile_features(corun PUBLIC cxx_std_17)
set_target_properties(corun PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(corun PROPERTIES COMPILE_FLAGS -O2)
target_include_directories(
    corun
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libcor/include>
)

if(CMAKE_SYSTEM_NAME MATCHES "SunOS")
target_link_libraries(
    corun
    PRIVATE
        LibSSH
        Boost::program_options
        libcor
        -lssp
)
else(CMAKE_SYSTEM_NAME MATCHES "SunOS")
target_link_libraries(
    corun
    PRIVATE
        LibSSH
        Boost::program_options
        libcor
)
endif(CMAKE_SYSTEM_NAME MATCHES "SunOS")

install(
    TARGETS
        corun
    RUNTIME
        DESTINATION bin
)