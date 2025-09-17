cmake_minimum_required(VERSION 3.27)

include(../library.cmake)

add_executable(
    kkmha
    http_parser.cpp
    server_cache_core.cpp
    server_default_handler.cpp
    server_kkmop_handler.cpp
    server_static_handler.cpp
    server_static_varop.cpp
    server_config_handler.cpp
    server_ping_handler.cpp
    server_core.cpp
    server_varop.cpp
    service_core.cpp
    service_varop.cpp
    kkmha_main.cpp
)

if (BUILD_SEPARATED)
    target_sources(kkmha PRIVATE kkmha_variables.cpp)
endif ()

#target_link_libraries(kkmha PRIVATE clang_rt.asan_dynamic-x86_64 clang_rt.asan_dynamic_runtime_thunk-x86_64)
#target_link_libraries(kkmha PRIVATE clang_rt.asan_dynamic-x86_64)
#target_link_libraries(kkmha PRIVATE clang_rt.ubsan_standalone_cxx-x86_64)

target_compile_definitions(kkmha PUBLIC ${XTXN_PUB_DEFS} PRIVATE ${XTXN_PRI_DEFS})
target_link_libraries(kkmha PRIVATE xtxn_lib)
target_link_libraries(kkmha PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(kkmha PRIVATE asio::asio)
target_link_libraries(kkmha PRIVATE nlohmann_json::nlohmann_json)
#target_link_libraries(kkmha INTERFACE <threads>)

if (WITH_ASAN)
    add_custom_command(
        TARGET kkmha
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CLANG_SANLIB_DIR}/${CLANG_ASAN_LIB}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
endif ()

if (BUILD_STATIC)
    #target_link_libraries(kkmha PRIVATE Advapi32.lib)
    target_link_libraries(kkmha PRIVATE Crypt32.lib)
    install(
        TARGETS kkmha
        COMPONENT kkmha
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
        DIRECTORIES ${CLANG_SANLIB_DIR}
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
        DIRECTORIES ${KKMHA_DEPS_VCPKG_DLL} ${CLANG_SANLIB_DIR}
        DESTINATION .
    )
endif ()

if (BUILD_TESTS)
    add_test(NAME test_kkmha COMMAND kkmha help)
endif ()
