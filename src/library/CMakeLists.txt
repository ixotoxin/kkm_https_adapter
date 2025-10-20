cmake_minimum_required(VERSION 3.28)

add_library(kkmha_lib OBJECT ${KKMHA_COMMON_SOURCE_FILES})

if (BUILD_SEPARATED)
    target_sources(kkmha_lib PRIVATE variables.cpp)
endif ()

target_compile_definitions(kkmha_lib PUBLIC ${KKMHA_PUBLIC_DEFS} PRIVATE ${KKMHA_PRIVATE_DEFS})
target_link_libraries(kkmha_lib PRIVATE nlohmann_json::nlohmann_json)
