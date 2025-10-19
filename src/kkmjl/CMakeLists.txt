cmake_minimum_required(VERSION 3.28)

add_executable(kkmjl kkmjl_main.cpp kkmjl_variables.cpp)
target_compile_definitions(kkmjl PUBLIC ${KKMHA_PUB_DEFS} PRIVATE ${KKMHA_PRI_DEFS})
target_link_libraries(kkmjl PRIVATE kkmha_lib)
target_link_libraries(kkmjl PRIVATE nlohmann_json::nlohmann_json)

install(
    TARGETS kkmjl
    COMPONENT kkmjl
    RUNTIME_DEPENDENCIES
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    DESTINATION .
)
