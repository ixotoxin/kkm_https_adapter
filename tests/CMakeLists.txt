cmake_minimum_required(VERSION 3.27)

find_package(Catch2 CONFIG REQUIRED PATHS ${KKMHA_DEPS_VCPKG})

set(TESTS_PUB_DEFS WIN32_LEAN_AND_MEAN NOMINMAX)

add_library(tests_lib OBJECT ${LIB_SOURCE_FILES})
target_compile_definitions(tests_lib PUBLIC ${TESTS_PUB_DEFS} JSON_USE_IMPLICIT_CONVERSIONS=0)
target_link_libraries(tests_lib PRIVATE nlohmann_json::nlohmann_json)

add_executable(test_lib_wconv lib_wconv.cpp)
target_compile_definitions(test_lib_wconv PUBLIC ${TESTS_PUB_DEFS})
target_link_libraries(test_lib_wconv PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_wconv COMMAND test_lib_wconv)

add_executable(test_lib_numeric lib_numeric.cpp)
target_compile_definitions(test_lib_numeric PUBLIC ${TESTS_PUB_DEFS})
target_link_libraries(test_lib_numeric PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_numeric COMMAND test_lib_numeric)

add_executable(test_lib_text lib_text.cpp)
target_compile_definitions(test_lib_text PUBLIC ${TESTS_PUB_DEFS})
target_link_libraries(test_lib_text PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_lib_text PRIVATE tests_lib)
add_test(NAME test_lib_text COMMAND test_lib_text)

add_executable(test_lib_datetime lib_datetime.cpp)
target_compile_definitions(test_lib_datetime PUBLIC ${TESTS_PUB_DEFS})
target_link_libraries(test_lib_datetime PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_lib_datetime PRIVATE tests_lib)
add_test(NAME test_lib_datetime COMMAND test_lib_datetime)

add_executable(test_lib_json lib_json.cpp)
target_compile_definitions(test_lib_json PUBLIC ${TESTS_PUB_DEFS} JSON_USE_IMPLICIT_CONVERSIONS=0)
target_link_libraries(test_lib_json PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_lib_json PRIVATE tests_lib)
add_test(NAME test_lib_json COMMAND test_lib_json)

add_executable(test_lib_utils lib_utils.cpp)
target_compile_definitions(test_lib_utils PUBLIC ${TESTS_PUB_DEFS})
target_link_libraries(test_lib_utils PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_utils COMMAND test_lib_utils)
