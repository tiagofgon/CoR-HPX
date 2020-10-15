target_sources(
    libcor
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/cor.cpp"
        # "${CMAKE_CURRENT_LIST_DIR}/message.cpp"
)

include(elements/CMakeLists.txt)
include(resources/CMakeLists.txt)
include(services/CMakeLists.txt)
include(system/CMakeLists.txt)
include(utils/CMakeLists.txt)
