target_sources(
    libcor
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/barrier.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/closure.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/domain.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/group.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/mutex.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/resource.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/resource_non_migrable.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/teste.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/teste_empty.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/closure_client.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/data_client.cpp"
        # "${CMAKE_CURRENT_LIST_DIR}/rwmutex.cpp"
)
