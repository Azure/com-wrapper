#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

if(NOT DEFINED CMAKE_MINIMUM_REQUIRED_VERSION)
    cmake_minimum_required(VERSION 3.18)
endif()

# canon way of using com_wrapper from another repo is below. It assumes the using repo has placed com_wrapper in "deps"

#if ((NOT TARGET com_wrapper) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/com_wrapper/CMakeLists.txt))
#    add_subdirectory(deps/com_wrapper)
#    include_directories(deps/com_wrapper/inc)
#endif() 

if (TARGET com_wrapper)
    RETURN()
endif()

project(com_wrapper)

#the following variables are project-wide and can be used with cmake-gui
option(run_unittests "set run_unittests to ON to run unittests (default is OFF)" OFF)
option(run_e2e_tests "set run_e2e_tests to ON to run e2e tests (default is OFF). Chsare dutility does not have any e2e tests, but the option needs to exist to evaluate in IF statements" OFF)
option(run_int_tests "set run_int_tests to ON to integration tests (default is OFF)." OFF)
option(run_perf_tests "set run_perf_tests to ON to build performance tests (default is OFF)." OFF)
option(run_reals_check "set run_reals_check to ON to run reals check (default is OFF)." OFF)
option(use_cppunittest "set use_cppunittest to ON to build CppUnitTest tests on Windows (default is OFF)" OFF)
option(run_traceability "run traceability tool (default is ON)" ON)

if((NOT "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo") AND (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    message(FATAL_ERROR "CMAKE_BUILD_TYPE must be specified and be precisely one of Debug or RelWithDebInfo")
endif()

# This is done because jemalloc is obtained from vcpkg and brought in with PkgConfig
# PkgConfig does not support defining different variables with the libraries to be linked for different configurations
# This forces us to only be able to generate one build configuration (either Debug or RelWithDebInfo)
# Setting CMAKE_CONFIGURATION_TYPES is the way to limit what configurations are generated
# so that the user gets only one configuration as option in the Visual Studio GUI
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}" CACHE STRING "" FORCE)

if (NOT DEFINED ENV{BUILD_BINARIESDIRECTORY})
    MESSAGE(FATAL_ERROR "cannot find BUILD_BINARIESDIRECTORY env variable which is absolutely needed. It can point to any folder where the binaries will pe placed. Here's an example: BUILD_BINARIESDIRECTORY=d:\\BUILD_BINARIESDIRECTORY")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG $ENV{BUILD_BINARIESDIRECTORY}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO $ENV{BUILD_BINARIESDIRECTORY}/RelWithDebInfo)

#bring in dependencies
#do not add or build any tests of the dependencies
set(original_run_e2e_tests ${run_e2e_tests})
set(original_run_int_tests ${run_int_tests})
set(original_run_perf_tests ${run_perf_tests})
set(original_run_unittests ${run_unittests})
set(original_run_traceability ${run_traceability})
set(original_run_reals_check ${run_reals_check})

set(run_e2e_tests OFF)
set(run_int_tests OFF)
set(run_perf_tests OFF)
set(run_unittests OFF)
set(run_traceability OFF)
set(run_reals_check OFF)

if ((WIN32) AND (${GBALLOC_LL_TYPE} STREQUAL "JEMALLOC"))
    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/vcpkg/scripts/buildsystems/vcpkg.cmake)
        set(VCPKG_OVERLAY_TRIPLETS "${CMAKE_CURRENT_LIST_DIR}/triplets")
        if(${fsanitize})
            set(VCPKG_TARGET_TRIPLET "x64-windows-static-comwrapper-asan")
        else()
            set(VCPKG_TARGET_TRIPLET "x64-windows-static-comwrapper")
        endif()

        include(deps/vcpkg/scripts/buildsystems/vcpkg.cmake)
    endif()

    find_package(PkgConfig REQUIRED)
    pkg_check_modules (JEMALLOC jemalloc)

    pkg_search_module(JEMALLOC REQUIRED jemalloc)
    include_directories(${JEMALLOC_INCLUDE_DIRS})

    #this wraps the jemalloc library in a target so it can be used in the rest of the build easily
    add_library(jemalloc INTERFACE)

    # The pkg_search_module above sets the variables needed for the include and link directories
    # we are interested to have the include directories produced by the pkg_search_module for jemalloc used so that
    # we can find the jemalloc headers
    # Also we need to link in our wrapper target the jemalloc static library, which lives at the location produced by
    # the pkg_search_module in ${pkgcfg_lib_JEMALLOC_jemalloc_s}
    # Note that PkgConfig is not geared to produce different variables pointing to different libs for different configurations
    # so the same variable will be used both debug and release (this forces us to only generate the CMakes for one build configuration only).
    target_link_libraries(jemalloc INTERFACE ${pkgcfg_lib_JEMALLOC_jemalloc_s})
    target_include_directories(jemalloc INTERFACE ${JEMALLOC_INCLUDE_DIRS})

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DJEMALLOC_NO_PRIVATE_NAMESPACE /D_REENTRANT /DJEMALLOC_EXPORT= /D_LIB")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DJEMALLOC_NO_PRIVATE_NAMESPACE /D_REENTRANT /DJEMALLOC_EXPORT= /D_LIB")
endif()

if ((NOT TARGET c_build_tools) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/c-build-tools/CMakeLists.txt))
    add_subdirectory(deps/c-build-tools)
    set_default_build_options()
endif()

if ((NOT TARGET macro_utils_c) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/macro-utils-c/CMakeLists.txt))
    add_subdirectory(deps/macro-utils-c)
    include_directories(${MACRO_UTILS_INC_FOLDER})
endif()

if ((NOT TARGET c_logging) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/c-logging/CMakeLists.txt))
    add_subdirectory(deps/c-logging)
    include_directories(deps/c-logging/v2/inc)
endif()

if ((NOT TARGET ctest) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/ctest/CMakeLists.txt))
    add_subdirectory(deps/ctest)
    include_directories(${CTEST_INC_FOLDER})
endif()

if ((NOT TARGET testrunnerswitcher) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/c-testrunnerswitcher/CMakeLists.txt))
    add_subdirectory(deps/c-testrunnerswitcher)
    include_directories(${TESTRUNNERSWITCHER_INC_FOLDER})
endif()

if ((NOT TARGET umock_c) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/umock-c/CMakeLists.txt))
    add_subdirectory(deps/umock-c)
    include_directories(${UMOCK_C_INC_FOLDER})
endif()

if ((NOT TARGET c_pal) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/c-pal/CMakeLists.txt))
    add_subdirectory(deps/c-pal)
    include_directories(${C_PAL_INC_FOLDER})
endif()

set(run_e2e_tests ${original_run_e2e_tests})
set(run_int_tests ${original_run_int_tests})
set(run_perf_tests ${original_run_perf_tests})
set(run_unittests ${original_run_unittests})
set(run_traceability ${original_run_traceability})
set(run_reals_check ${original_run_reals_check})

include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)

FILE(GLOB com_wrapper_md_files "devdoc/*.md")
SOURCE_GROUP(devdoc FILES ${com_wrapper_md_files})

add_library(com_wrapper INTERFACE)
target_include_directories(com_wrapper INTERFACE ${CMAKE_CURRENT_LIST_DIR}/inc)

add_subdirectory(tests)

if(${run_reals_check})
    add_reals_check_target()
endif()
