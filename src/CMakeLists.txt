cmake_minimum_required(VERSION 3.27)

configure_file(kkmha_config.h.in kkmha_config.h)

add_executable(
    kkmha
    kkmha_main.cpp
    library/datetime.cpp
    library/text.cpp
    library/utils.cpp
    config.cpp
    config_handler.cpp
    default_handler.cpp
    http_cache.cpp
    http_parser.cpp
    kkm.cpp
    kkm_handler.cpp
    log.cpp
    ping_handler.cpp
    server.cpp
    service.cpp
    static_handler.cpp
)
target_compile_definitions(
    kkmha
    PUBLIC
        WIN32_LEAN_AND_MEAN
        ASIO_STANDALONE
        ASIO_NO_WIN32_LEAN_AND_MEAN
        ASIO_NO_NOMINMAX
        ASIO_HAS_THREADS
        ASIO_HAS_STD_COROUTINE
        ASIO_HAS_CO_AWAIT
)
target_include_directories(kkmha PRIVATE "${PROJECT_BINARY_DIR}/src")
target_include_directories(kkmha PRIVATE ${KKMHA_DEPS_FPTR10_INCL})
target_link_libraries(kkmha PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(kkmha PRIVATE asio::asio)
target_link_libraries(kkmha PRIVATE nlohmann_json::nlohmann_json)
#target_link_libraries(kkmha INTERFACE <threads>)
add_custom_command(
    TARGET kkmha POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${KKMHA_DEPS_VCPKG_DLL} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

if (BUILD_TESTS)
    add_test(NAME test_kkmha COMMAND kkmha help)
endif ()

install(
    TARGETS kkmha
    COMPONENT kkmha
    RUNTIME_DEPENDENCIES
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    DIRECTORIES ${KKMHA_DEPS_VCPKG_DLL}
    DESTINATION .
)
