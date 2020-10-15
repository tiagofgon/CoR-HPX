add_executable(corx corx.cpp)

target_compile_features(corx PUBLIC cxx_std_17)
set_target_properties(corx PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(corx PROPERTIES COMPILE_FLAGS -O2)

if(CMAKE_SYSTEM_NAME MATCHES "SunOS")
target_link_libraries(
    corx
    PRIVATE
        libcor
        Spread
        SSRCSpread
        LibEvent
        Cereal
        LibSSH
        -lssp
)
else(CMAKE_SYSTEM_NAME MATCHES "SunOS")
target_link_libraries(
    corx
    PRIVATE
        libcor
        Spread
        SSRCSpread
        LibEvent
        Cereal
        LibSSH
        -ldl
        -lpthread
)
endif(CMAKE_SYSTEM_NAME MATCHES "SunOS")

install(
    TARGETS
        corx
    RUNTIME
        DESTINATION bin
)