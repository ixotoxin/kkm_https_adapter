cmake_minimum_required(VERSION 3.27)

include(../library.cmake)

add_executable(kkmop kkmop_main.cpp kkmop_variables.cpp)
target_compile_definitions(kkmop PUBLIC ${XTXN_PUB_DEFS} PRIVATE ${XTXN_PRI_DEFS})
target_link_libraries(kkmop PRIVATE xtxn_lib)
target_link_libraries(kkmop PRIVATE nlohmann_json::nlohmann_json)

install(
    TARGETS kkmop
    COMPONENT kkmop
    RUNTIME_DEPENDENCIES
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    DESTINATION .
)

if (BUILD_TESTS)
    add_test(NAME test_kkmop COMMAND kkmop help)
endif ()
