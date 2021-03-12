file(
    GLOB_RECURSE
    ALL_CXX_SOURCE_FILES
    src/*.cpp include/*.hpp
    )

find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    add_custom_target(
        clang-format
        COMMAND clang-format
        -i
        ${ALL_CXX_SOURCE_FILES}
    )
endif()