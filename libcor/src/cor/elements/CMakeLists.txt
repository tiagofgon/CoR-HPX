target_sources(
    libcor
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/container.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/dynamic_organizer.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/sbarrier.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/smutex.cpp"
        # "${CMAKE_CURRENT_LIST_DIR}/srwmutex.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/static_organizer.cpp"
)
