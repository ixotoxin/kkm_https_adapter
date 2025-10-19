cmake_minimum_required(VERSION 3.28)

add_library(
    kkmha_lib
    OBJECT
        ${KKMHA_LIB_SOURCE_FILES}
        ${KKMHA_MAIN_SOURCE_FILES}
        ${KKMHA_LOG_SOURCE_FILES}
        ${KKMHA_KKM_SOURCE_FILES}
        ${KKMHA_CONFIG_SOURCE_FILES}
)

if (BUILD_SEPARATED)
    target_sources(kkmha_lib PRIVATE variables.cpp)
endif ()

target_compile_definitions(kkmha_lib PUBLIC ${KKMHA_PUB_DEFS} PRIVATE ${KKMHA_PRI_DEFS})
target_link_libraries(kkmha_lib PRIVATE nlohmann_json::nlohmann_json)
