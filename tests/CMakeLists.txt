cmake_minimum_required(VERSION 3.28)

find_package(Catch2 CONFIG REQUIRED PATHS "${KKMHA_DEPS_VCPKG}")

set(TESTS_PUB_DEFS WIN32_LEAN_AND_MEAN NOMINMAX)

add_library(tests_lib OBJECT ${LIB_SOURCE_FILES})
target_compile_definitions(tests_lib PUBLIC ${TESTS_PUB_DEFS} JSON_USE_IMPLICIT_CONVERSIONS=0)
target_link_libraries(tests_lib PRIVATE nlohmann_json::nlohmann_json)

add_executable(test_lib_wconv lib_wconv.cpp)
target_compile_definitions(test_lib_wconv PUBLIC ${TESTS_PUB_DEFS})
target_link_libraries(test_lib_wconv PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_wconv COMMAND test_lib_wconv)

add_executable(test_lib_defer lib_defer.cpp)
target_compile_definitions(test_lib_defer PUBLIC ${TESTS_PUB_DEFS})
target_link_libraries(test_lib_defer PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_defer COMMAND test_lib_defer)

add_executable(test_lib_numeric lib_numeric.cpp)
target_compile_definitions(test_lib_numeric PUBLIC ${TESTS_PUB_DEFS})
target_link_libraries(test_lib_numeric PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_numeric COMMAND test_lib_numeric)

add_executable(test_lib_datetime lib_datetime.cpp)
target_compile_definitions(test_lib_datetime PUBLIC ${TESTS_PUB_DEFS})
target_link_libraries(test_lib_datetime PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_lib_datetime PRIVATE tests_lib)
add_test(NAME test_lib_datetime COMMAND test_lib_datetime)

add_executable(test_lib_text lib_text.cpp)
target_compile_definitions(test_lib_text PUBLIC ${TESTS_PUB_DEFS})
target_link_libraries(test_lib_text PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_lib_text PRIVATE tests_lib)
add_test(NAME test_lib_text COMMAND test_lib_text)

add_executable(test_lib_path lib_path.cpp)
target_compile_definitions(test_lib_path PUBLIC ${TESTS_PUB_DEFS})
target_link_libraries(test_lib_path PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_path COMMAND test_lib_path)

add_executable(test_lib_json lib_json.cpp)
target_compile_definitions(test_lib_json PUBLIC ${TESTS_PUB_DEFS} JSON_USE_IMPLICIT_CONVERSIONS=0)
target_link_libraries(test_lib_json PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_lib_json PRIVATE tests_lib)
add_test(NAME test_lib_json COMMAND test_lib_json)
