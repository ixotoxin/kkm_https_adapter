cmake_minimum_required(VERSION 3.27)

add_library(lib_kkmha OBJECT datetime.cpp text.cpp utils.cpp)
target_compile_definitions(lib_kkmha PUBLIC ${KKMHA_PUB_DEFS})
