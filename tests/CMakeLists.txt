cmake_minimum_required(VERSION 3.27)

find_package(Catch2 CONFIG REQUIRED PATHS ${KKMHA_DEPS_VCPKG})

include_directories("${CMAKE_SOURCE_DIR}/src")

add_executable(test_lib_wconv lib_wconv.cpp)
target_link_libraries(test_lib_wconv PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
add_test(NAME test_lib_wconv COMMAND test_lib_wconv)

add_executable(test_lib_numeric lib_numeric.cpp)
target_link_libraries(test_lib_numeric PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
add_test(NAME test_lib_numeric COMMAND test_lib_numeric)

add_executable(test_lib_text lib_text.cpp ../src/library/text.cpp)
target_link_libraries(test_lib_text PRIVATE Catch2::Catch2)
add_test(NAME test_lib_text COMMAND test_lib_text)

add_executable(test_lib_datetime lib_datetime.cpp ../src/library/datetime.cpp)
target_link_libraries(test_lib_datetime PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_datetime COMMAND test_lib_datetime)

add_executable(test_lib_json lib_json.cpp ../src/library/text.cpp)
target_link_libraries(test_lib_json PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
add_test(NAME test_lib_json COMMAND test_lib_json)

add_executable(test_lib_utils lib_utils.cpp)
target_link_libraries(test_lib_utils PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
add_test(NAME test_lib_utils COMMAND test_lib_utils)
