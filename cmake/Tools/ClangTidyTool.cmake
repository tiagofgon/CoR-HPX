find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
    add_custom_target(
        clang-tidy
        COMMAND python3
        /usr/local/opt/llvm/share/clang/run-clang-tidy.py
        -header-filter='.*'
        -checks='*'
        -extra-arg=-std=c++11
    )
endif()
