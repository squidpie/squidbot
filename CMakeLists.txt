cmake_minimum_required(VERSION 3.18.4)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -fPIC")# -fsanitize=address")

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
    ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/core/service ${CMAKE_SOURCE_DIR}/src/core/events
    ${CMAKE_SOURCE_DIR}/src/core/runner ${CMAKE_SOURCE_DIR}/src/services ${CMAKE_SOURCE_DIR}/src/core/plugin)

add_subdirectory(src/core)
add_subdirectory(src)
#add_subdirectory(src/services/obs)
add_subdirectory(src/services/mockservice)
add_subdirectory(src/plugins/mockplugin)

# Google Test Stuff
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/3fa7f983c69f780378b4d1ad44d36030ca951ba6.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    squidtest
    src/squidbot_test.cpp
    ${CORE_TEST_SOURCES} ${MOCK_SERVICE_SOURCES} ${MOCKPLUGIN_SOURCES})

target_link_libraries(
    squidtest
    PUBLIC
    LIB_CORE)

target_link_libraries(
    squidtest
    PRIVATE
    gtest
    gmock
    gtest_main
    ${CMAKE_DL_LIBS})

set_target_properties(
    squidtest
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/test"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/test"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test")

target_compile_definitions(
    squidtest
    PUBLIC
    _GTEST_COMPILE)