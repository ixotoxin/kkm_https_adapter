cmake_minimum_required(VERSION 3.28)

include(../library.cmake)

add_executable(kkmjl kkmjl_main.cpp kkmjl_variables.cpp)
target_compile_definitions(kkmjl PUBLIC ${XTXN_PUB_DEFS} PRIVATE ${XTXN_PRI_DEFS})
target_link_libraries(kkmjl PRIVATE xtxn_lib)
target_link_libraries(kkmjl PRIVATE nlohmann_json::nlohmann_json)

install(
    TARGETS kkmjl
    COMPONENT kkmjl
    RUNTIME_DEPENDENCIES
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    DESTINATION .
)

#if (BUILD_TESTS)
#    add_test(NAME test_kkmjl COMMAND kkmjl help)
#endif ()
