add_library(pool SHARED pool.cpp)
target_compile_features(pool PUBLIC cxx_std_17)
set_target_properties(pool PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(pool PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(pool PROPERTIES COMPILE_FLAGS -O2)
target_link_libraries(
    pool
    PRIVATE
        libcor
	    HPX::hpx
)

install(
    TARGETS
        pool
    LIBRARY
        DESTINATION lib
)
