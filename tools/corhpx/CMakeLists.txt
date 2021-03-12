add_hpx_executable(corhpx 
	SOURCES corhpx.cpp
	DEPENDENCIES 
		libcor
		LibSSH
		-ldl
		-lpthread
	COMPONENT_DEPENDENCIES iostreams
)


target_compile_features(corhpx PUBLIC cxx_std_17)
set_target_properties(corhpx PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(corhpx PROPERTIES COMPILE_FLAGS -O2)


install(
    TARGETS
        corhpx
    RUNTIME
        DESTINATION bin
)
