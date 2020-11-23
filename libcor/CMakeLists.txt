add_library(libcor SHARED "")

target_compile_features(libcor PUBLIC cxx_std_17)
set_target_properties(libcor PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(libcor PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(libcor PROPERTIES LIBRARY_OUTPUT_NAME "cor")
set_target_properties(libcor PROPERTIES COMPILE_FLAGS -O2)

target_include_directories(
    libcor
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libcor/include>
        $<INSTALL_INTERFACE:include>
)

# Add this compile definition for debug builds, this same logic works for
# target_compile_options, target_link_options, etc.
target_compile_definitions(libcor PRIVATE
    $<$<CONFIG:Debug>:
        LIBCOR_DEBUG=1
    >
)

target_link_libraries(
    libcor
    PRIVATE
        # Spread
        # SSRCSpread
        # LibEvent
        # LibEventThreads
        # Cereal
        LibSSH
    PUBLIC
	    HPX::hpx
)

install(
    TARGETS
        libcor
    LIBRARY
        DESTINATION lib
)

add_subdirectory(src)
