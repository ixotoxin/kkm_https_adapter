cmake_minimum_required(VERSION 3.27)

include(../library.cmake)

add_library(xtxn_lib OBJECT ${LIB_SOURCE_FILES} ${LOG_SOURCE_FILES} ${KKM_SOURCE_FILES} ${CONFIG_SOURCE_FILES})

if (BUILD_SEPARATED)
    target_sources(xtxn_lib PRIVATE variables.cpp)
endif ()

target_compile_definitions(xtxn_lib PUBLIC ${XTXN_PUB_DEFS} PRIVATE ${XTXN_PRI_DEFS})
target_link_libraries(xtxn_lib PRIVATE nlohmann_json::nlohmann_json)
