cmake_minimum_required(VERSION 3.28)
project(kkm_https_adapter VERSION 1.7.10 LANGUAGES CXX)

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
#option(WITH_ASAN "Build with AddressSanitizer" OFF)
#option(WITH_UBSAN "Build with UndefinedBehaviorSanitizer" OFF)
option(WITH_CRTDBG "Build with memory profiling" OFF)
option(WITH_LEAKS "Build with memory leaks" OFF)
option(WITH_RELSL "Enable relative path for source location" ON)
option(WITH_SSIAC "Enable std::string invasive access" OFF)

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

find_package(OpenSSL REQUIRED PATHS "${KKMHA_DEPS_VCPKG}")
find_package(asio REQUIRED PATHS "${KKMHA_DEPS_VCPKG}")
find_package(nlohmann_json REQUIRED PATHS "${KKMHA_DEPS_VCPKG}")

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
    if (WITH_RELSL)
        add_compile_options("-fmacro-prefix-map=${CMAKE_CURRENT_SOURCE_DIR}/=.\\")
    endif ()
    if (BUILD_STATIC)
        add_compile_options(-static)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    endif ()
endif ()

add_definitions(/DUNICODE /D_UNICODE /DWIN32_LEAN_AND_MEAN /DNOMINMAX /D_WIN32_WINNT=0x0601)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/fptr10")

set(KKMHA_CONFIG_INCLUDE_DIR "${PROJECT_BINARY_DIR}/_Include_${CMAKE_BUILD_TYPE}")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/options.h.in" "${KKMHA_CONFIG_INCLUDE_DIR}/cmake/options.h")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/variables.h.in" "${KKMHA_CONFIG_INCLUDE_DIR}/cmake/variables.h")
include_directories("${KKMHA_CONFIG_INCLUDE_DIR}" BEFORE)

file(GLOB_RECURSE KKMHA_LIBRARY_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/library/lib/*.cpp")
file(GLOB_RECURSE KKMHA_COMMON_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/library/*/*.cpp")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/library" BEFORE)

if (BUILD_TESTS)
    enable_testing()
endif ()

set(
    KKMHA_PUBLIC_DEFS
        ASIO_STANDALONE
        ASIO_NO_WIN32_LEAN_AND_MEAN
        ASIO_NO_NOMINMAX
        ASIO_HAS_THREADS
        ASIO_HAS_STD_COROUTINE
        ASIO_HAS_CO_AWAIT
        JSON_USE_IMPLICIT_CONVERSIONS=0
)

if (BUILD_SEPARATED)
    set(
        KKMHA_PRIVATE_DEFS
            EXTERNAL_LIB_VARIABLES
            EXTERNAL_MAIN_VARIABLES
            EXTERNAL_LOG_VARIABLES
            EXTERNAL_KKM_VARIABLES
            EXTERNAL_CONFIG_VARIABLES
    )
else ()
    set(KKMHA_PRIVATE_DEFS)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/kkmop")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/kkmjl")
endif ()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/library")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/kkmha")

if (BUILD_SEPARATED)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/kkmop")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/kkmjl")
endif ()

if (BUILD_TESTS)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif ()

#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/heap")
