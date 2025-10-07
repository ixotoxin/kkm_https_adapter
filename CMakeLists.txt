cmake_minimum_required(VERSION 3.28)
project(kkm_https_adapter VERSION 1.7.3 LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_FIND_DEBUG_MODE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "$<1:${PROJECT_BINARY_DIR}/_Archive_${CMAKE_BUILD_TYPE}>" CACHE PATH "Archive")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<1:${PROJECT_BINARY_DIR}/_Binary_${CMAKE_BUILD_TYPE}>" CACHE PATH "Binary")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<1:${PROJECT_BINARY_DIR}/_Library_${CMAKE_BUILD_TYPE}>" CACHE PATH "Library")

option(BUILD_SEPARATED "Build separated" OFF)
option(BUILD_STATIC "Build static" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(ENABLE_HACKS "Enable questionable optimizations" OFF)
#option(WITH_ASAN "Build with AddressSanitizer" OFF)
#option(WITH_UBSAN "Build with UndefinedBehaviorSanitizer" OFF)
option(WITH_CRTDBG "Build with memory profiling" OFF)
option(WITH_LEAKS "Build with memory leaks" OFF)

string(TIMESTAMP KKMHA_BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")
set(KKMHA_BUILD_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(KKMHA_DEPS_FPTR10 "${CMAKE_CURRENT_SOURCE_DIR}/deps/fptr10")

set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

if (BUILD_STATIC)
    message(STATUS "Build static executables")
    set(KKMHA_BUILD_TYPE "Static/${CMAKE_BUILD_TYPE}")
    set(KKMHA_DEPS_VCPKG "${CMAKE_CURRENT_SOURCE_DIR}/deps/static/vcpkg_installed/x64-windows-static")
    if (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
    else ()
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
    endif ()
else ()
    set(KKMHA_BUILD_TYPE "Dynamic/${CMAKE_BUILD_TYPE}")
    set(KKMHA_DEPS_VCPKG "${CMAKE_CURRENT_SOURCE_DIR}/deps/dynamic/vcpkg_installed/x64-windows")
    if (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDebugDLL)
        set(KKMHA_DEPS_VCPKG_DLL "${KKMHA_DEPS_VCPKG}/debug/bin")
    else ()
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDLL)
        set(KKMHA_DEPS_VCPKG_DLL "${KKMHA_DEPS_VCPKG}/bin")
    endif ()
endif ()

set(OPENSSL_ROOT_DIR ${KKMHA_DEPS_VCPKG})

find_package(OpenSSL REQUIRED PATHS ${KKMHA_DEPS_VCPKG})
find_package(asio REQUIRED PATHS ${KKMHA_DEPS_VCPKG})
find_package(nlohmann_json REQUIRED PATHS ${KKMHA_DEPS_VCPKG})

set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
set(JSON_ImplicitConversions OFF CACHE INTERNAL "")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Ob0 /Oy- /RTC1 /Zi /DDEBUG /D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 /DNDEBUG")
    add_compile_options(/W4 /WX /EHsc /Zc:preprocessor /utf-8 /await:strict)
    add_compile_options(/wd4068 /wd4244 /wd4267 /wd4310 /wd4996)
else ()
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG -D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    add_compile_options(
        -nostdlib -nodefaultlibs
        -Werror -Wall -Wextra -Wpedantic
        -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wfloat-equal
        -Wnon-virtual-dtor -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo
        -Wno-deprecated-declarations -Wno-implicit-int-conversion
        -Wno-sign-compare -Wno-sign-conversion -Wno-unknown-pragmas
    )
    if (BUILD_STATIC)
        add_compile_options(-static)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    endif ()
endif ()

add_definitions(/DUNICODE /D_UNICODE /DWIN32_LEAN_AND_MEAN /D_WIN32_WINNT=0x0601)

set(STD_STRING_INVASIVE_ACCESS ${ENABLE_HACKS})

include("${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/incl.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/src/library/incl.cmake")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/fptr10")

if (BUILD_TESTS)
    enable_testing()
endif ()

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/heap)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/library)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/kkmha)
if (BUILD_SEPARATED)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/kkmop)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/kkmjl)
endif ()

if (BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()
