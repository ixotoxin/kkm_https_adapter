cmake_minimum_required(VERSION 3.27)

add_executable(
    kkmha
    kkmha_main.cpp
    library/text.cpp library/datetime.cpp library/utils.cpp log.cpp config.cpp kkm.cpp
    http_cache.cpp kkm_handler.cpp static_handler.cpp config_handler.cpp ping_handler.cpp default_handler.cpp
    http_parser.cpp server.cpp service.cpp
)
target_compile_definitions(kkmha PUBLIC WIN32_LEAN_AND_MEAN ASIO_STANDALONE ASIO_HAS_CO_AWAIT ASIO_HAS_STD_COROUTINE ASIO_HAS_THREADS)
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
