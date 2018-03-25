cmake_minimum_required(VERSION 3.9)

project(VulkanSystemError)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if($ENV{TRAVIS})
    if($ENV{TRAVIS} STREQUAL "true")
        message(STATUS "Building on Travis-CI.")
        set(IS_TRAVIS_BUILD TRUE)
    endif()
endif()

if(MSVC)
    option(GB_PARALLEL_BUILD "If set, uses multiple build processes in parallel." ON)
    if(GB_PARALLEL_BUILD)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    endif()
endif()

set(VULKAN_SYSTEM_ERROR_VERSION_MAJOR 0)
set(VULKAN_SYSTEM_ERROR_VERSION_MINOR 1)
set(VULKAN_SYSTEM_ERROR_VERSION_PATCH 0)
set(VULKAN_SYSTEM_ERROR_VERSION "${VULKAN_SYSTEM_ERROR_VERSION_MAJOR}.${VULKAN_SYSTEM_ERROR_VERSION_MINOR}.${VULKAN_SYSTEM_ERROR_VERSION_PATCH}")

find_package(Vulkan REQUIRED)

set(VULKAN_SYSTEM_ERROR_SOURCE_DIR ${PROJECT_SOURCE_DIR}/vulkan_system_error)
set(VULKAN_SYSTEM_ERROR_TEST_DIR ${PROJECT_SOURCE_DIR}/test)

set(VULKAN_SYSTEM_ERROR_SOURCE_FILES
    ${VULKAN_SYSTEM_ERROR_SOURCE_DIR}/vulkan_system_error.hpp
)

set(VULKAN_SYSTEM_ERROR_TEST_SOURCES
    ${VULKAN_SYSTEM_ERROR_TEST_DIR}/catch_main.cpp
    ${VULKAN_SYSTEM_ERROR_TEST_DIR}/test_main.cpp
    ${VULKAN_SYSTEM_ERROR_TEST_DIR}/test_system_error.cpp
)

add_library(vulkan_system_error INTERFACE)
target_sources(vulkan_system_error INTERFACE ${VULKAN_SYSTEM_ERROR_SOURCE_FILES})
target_include_directories(vulkan_system_error INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)
target_link_libraries(vulkan_system_error INTERFACE Vulkan::Vulkan)

###############################################################################
## Testing
###############################################################################

option(VULKAN_SYSTEM_ERROR_BUILD_TESTS "Determines whether to build tests." ON)
if(VULKAN_SYSTEM_ERROR_BUILD_TESTS)
    enable_testing()

    if(NOT TARGET Catch)
        include(ExternalProject)
        if(IS_TRAVIS_BUILD)     # on travis, use git for fetching instead of wget
            set(VULKAN_SYSTEM_ERROR_FETCH_EXTERNAL_CATCH
                GIT_REPOSITORY https://github.com/philsquared/Catch.git
                GIT_TAG v2.1.2)
        elseif(WIN32)
            set(VULKAN_SYSTEM_ERROR_FETCH_EXTERNAL_CATCH
                URL https://github.com/catchorg/Catch2/archive/v2.1.2.zip
                URL_HASH MD5=54d2785d7e5549907421e8db5c1b3817)
        else()
            set(VULKAN_SYSTEM_ERROR_FETCH_EXTERNAL_CATCH
                URL https://github.com/catchorg/Catch2/archive/v2.1.2.tar.gz
                URL_HASH MD5=4c5740d7446d9b4e3735dcff0951aaca)
        endif()
        ExternalProject_Add(Catch-External
            PREFIX ${CMAKE_BINARY_DIR}/external/Catch
            ${VULKAN_SYSTEM_ERROR_FETCH_EXTERNAL_CATCH}
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/external/Catch/src/Catch-External/single_include/catch.hpp
                                                     ${CMAKE_BINARY_DIR}/external/Catch/include/catch.hpp
        )
        add_library(Catch INTERFACE)
        add_dependencies(Catch Catch-External)
        target_include_directories(Catch INTERFACE ${CMAKE_BINARY_DIR}/external/Catch/include)
    endif()

    add_executable(vulkan_system_error_test ${VULKAN_SYSTEM_ERROR_TEST_SOURCES})
    target_link_libraries(vulkan_system_error_test PUBLIC vulkan_system_error Catch)

    target_compile_options(vulkan_system_error_test INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/W4>)
    target_compile_options(vulkan_system_error_test INTERFACE $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/permissive->)
    target_compile_options(vulkan_system_error_test INTERFACE $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/std:c++latest>)
    target_compile_options(vulkan_system_error_test INTERFACE
        $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>:-pedantic -Wall>)
    target_compile_options(vulkan_system_error_test INTERFACE $<$<CXX_COMPILER_ID:GNU>:-pedantic -Wall>)
    
    add_test(NAME TestVulkanSystemError COMMAND vulkan_system_error_test)
endif()
