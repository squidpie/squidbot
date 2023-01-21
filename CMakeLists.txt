# CMakeLists.txt
# Copyright (C) 2023  Squidpie

cmake_minimum_required(VERSION 3.18.4)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -fPIC -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -fPIC")

project(Squidbot
        VERSION 1
        DESCRIPTION "Stream Automation Framekwork")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# export compile database for fleet
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXTERNAL_LIB_DIR ${CMAKE_SOURCE_DIR}/external)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(
    plog
    URL https://github.com/SergiusTheBest/plog/archive/refs/heads/master.zip
)
FetchContent_Populate(plog)
set(LIB_PLOG ${plog_SOURCE_DIR}/include)
message(STATUS "plog is loaded at " ${LIB_PLOG})

include_directories(
    ${LIB_PLOG}
    ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/src/logging
    ${CMAKE_SOURCE_DIR}/src/core/service ${CMAKE_SOURCE_DIR}/src/core/events
    ${CMAKE_SOURCE_DIR}/src/core/runner ${CMAKE_SOURCE_DIR}/src/core/plugin
    ${CMAKE_SOURCE_DIR}/src/services ${CMAKE_SOURCE_DIR}/src/plugins)

add_subdirectory(src)
add_subdirectory(src/core)
#add_subdirectory(src/services/obs)
add_subdirectory(src/services/mockservice)
add_subdirectory(src/plugins/mockplugin)
add_subdirectory(src/plugins/admin)
add_subdirectory(src/logging/logger)

# Google Test Stuff
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/3fa7f983c69f780378b4d1ad44d36030ca951ba6.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set_target_properties(
  gtest
  PROPERTIES
  EXCLUDE_FROM_ALL True)

set_target_properties(
  gmock
  PROPERTIES
  EXCLUDE_FROM_ALL True)

set_target_properties(
  gtest_main
  PROPERTIES
  EXCLUDE_FROM_ALL True)

set_target_properties(
  gmock_main
  PROPERTIES
  EXCLUDE_FROM_ALL True)

add_executable(
    squidtest
    src/gtest_main.cpp
    src/squidbot_test.cpp src/logging/logging.cpp
    ${CORE_TEST_SOURCES} ${MOCK_SERVICE_SOURCES} ${MOCKPLUGIN_SOURCES})

add_dependencies(
  squidtest
  ploginit mockservice mockplugin gtest gmock gtest_main gmock_main)

target_link_libraries(
    squidtest
    PUBLIC
    LIB_CORE)

target_link_libraries(
    squidtest
    PRIVATE
    gtest
    gmock
    #gtest_main
    ${CMAKE_DL_LIBS})

set_target_properties(
    squidtest
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
    EXCLUDE_FROM_ALL True)

target_compile_definitions(
    squidtest
    PUBLIC
    _GTEST_COMPILE)