cmake_minimum_required(VERSION 3.27)
project(kkm_https_adapter VERSION 1.2.0 LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/_arc_bucket_ CACHE PATH "Bucket")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/_bin_bucket_ CACHE PATH "Bucket")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/_lib_bucket_ CACHE PATH "Bucket")

option(BUILD_STATIC "Build static" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(WITH_MEMORY_PROFILING "Build with memory profiling (Debug only)" OFF)
option(WITH_MEMORY_LEAKS "Build with memory leaks (Debug only)" OFF)

#set(VCPKG_TRACE_FIND_PACKAGE ON)
#MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

set(KKMHA_DEPS_VCPKG "${CMAKE_SOURCE_DIR}/deps/vcpkg_installed/x64-windows")
set(KKMHA_DEPS_FPTR10_INCL "${CMAKE_SOURCE_DIR}/deps/fptr10")

#if (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
#    set(KKMHA_DEPS_VCPKG_DLL "${KKMHA_DEPS_VCPKG}/debug/bin")
#else ()
    set(KKMHA_DEPS_VCPKG_DLL "${KKMHA_DEPS_VCPKG}/bin")
#endif ()

set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

set(OPENSSL_ROOT_DIR ${KKMHA_DEPS_VCPKG})
#if (BUILD_STATIC)
#    set(OPENSSL_USE_STATIC_LIBS TRUE)
#endif ()

find_package(OpenSSL REQUIRED PATHS ${KKMHA_DEPS_VCPKG})
find_package(asio REQUIRED PATHS ${KKMHA_DEPS_VCPKG})
find_package(nlohmann_json REQUIRED PATHS ${KKMHA_DEPS_VCPKG})

set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
set(JSON_ImplicitConversions OFF CACHE INTERNAL "")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DJSON_USE_IMPLICIT_CONVERSIONS=0")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Ob0 /Oy- /RTC1 /Zi /EHsc /DDEBUG /D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 /DNDEBUG")
    add_compile_options(/Zc:preprocessor /W4 /WX /EHsc /utf-8 /await:strict)
    add_compile_options(/wd4068 /wd4324 /wd4996)
    add_compile_options(/wd4244 /wd4310 /wd4267)
    add_definitions(/DUNICODE /D_UNICODE /D_WIN32_WINNT=0x0601)
    if (BUILD_STATIC)
        message(STATUS "Build static executables")
        if (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
            add_compile_options(/MTd)
        else ()
            add_compile_options(/MT)
        endif ()
    else ()
        if (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
            add_compile_options(/MDd)
        else ()
            add_compile_options(/MD)
        endif ()
    endif ()
    if (WITH_MEMORY_PROFILING)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DWITH_MEMORY_PROFILING")
        if (WITH_MEMORY_LEAKS)
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DWITH_MEMORY_LEAKS")
        endif ()
    endif ()
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DJSON_USE_IMPLICIT_CONVERSIONS=0")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG -D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    add_compile_options(
        -nostdlib
        -Werror
        -Wall
        -Wextra
        -Wpedantic
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        #-Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        #-Wsign-conversion
        -Wsign-promo
        -Wno-implicit-int-conversion
        -Wno-shorten-64-to-32
        -Wno-sign-compare
        -Wno-sign-conversion
        -Wno-deprecated-declarations
        -Wno-unknown-pragmas
    )
    if (BUILD_STATIC)
        message(STATUS "Build static executables")
        add_compile_options(-static)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    endif ()
    if (WITH_MEMORY_PROFILING)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DWITH_MEMORY_PROFILING")
        if (WITH_MEMORY_LEAKS)
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DWITH_MEMORY_LEAKS")
        endif ()
    endif ()
endif ()

message(STATUS "+--")
message(STATUS "|         CMAKE_CXX_FLAGS = '${CMAKE_CXX_FLAGS}'")
message(STATUS "|   CMAKE_CXX_FLAGS_DEBUG = '${CMAKE_CXX_FLAGS_DEBUG}'")
message(STATUS "| CMAKE_CXX_FLAGS_RELEASE = '${CMAKE_CXX_FLAGS_RELEASE}'")
message(STATUS "|  CMAKE_EXE_LINKER_FLAGS = '${CMAKE_EXE_LINKER_FLAGS}'")
message(STATUS "+--")

if (BUILD_TESTS)
    enable_testing()
endif ()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

if (BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()
