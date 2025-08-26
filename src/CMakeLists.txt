cmake_minimum_required(VERSION 3.27)

add_executable(
    kkmha
    kkmha_main.cpp
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

target_compile_definitions(kkmha PUBLIC ${KKMHA_PUB_DEFS})
target_link_libraries(kkmha PRIVATE lib_kkmha)
target_link_libraries(kkmha PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(kkmha PRIVATE asio::asio)
target_link_libraries(kkmha PRIVATE nlohmann_json::nlohmann_json)
#target_link_libraries(kkmha INTERFACE <threads>)

if (BUILD_STATIC)
    find_path(CRYPT_INCLUDE_DIR wincrypt.h)
    find_library(CRYPT_LIBRARY crypt32)
    target_link_libraries(kkmha PRIVATE crypt32.lib)
    install(
        TARGETS kkmha
        COMPONENT kkmha
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
        DESTINATION .
    )
else ()
    add_custom_command(
        TARGET kkmha
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${KKMHA_DEPS_VCPKG_DLL}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
    install(
        TARGETS kkmha
        COMPONENT kkmha
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
        DIRECTORIES ${KKMHA_DEPS_VCPKG_DLL}
        DESTINATION .
    )
endif ()

if (BUILD_TESTS)
    add_test(NAME test_kkmha COMMAND kkmha help)
endif ()
