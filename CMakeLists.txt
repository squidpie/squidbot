cmake_minimum_required(VERSION 3.18.4)

project(Squidbot
        VERSION 1
        DESCRIPTION "Stream Automation Framekwork")

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# export compile database for fleet
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Google Test Stuff
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/3fa7f983c69f780378b4d1ad44d36030ca951ba6.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_SOURCE_FILES
    src/squidbot_test.cpp src/services/service_test.cpp src/services/obs/obs_test.cpp
    src/services/service_manager_test.cpp)

set(TEST_IMPL_FILES
    src/services/obs/obs.cpp src/services/service_manager.cpp src/services/service.cpp)

add_executable(
    squidtest
    ${TEST_SOURCE_FILES} ${TEST_IMPL_FILES})

target_link_libraries(
    squidtest
    PRIVATE
    gtest
    gmock
    gtest_main
    ${CMAKE_DL_LIBS})

target_include_directories(
    squidtest
    PRIVATE
    src src/services src/services/obs)

set_target_properties(
    squidtest
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/test"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/test"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test")

add_subdirectory(src)
add_subdirectory(src/services/obs)
add_subdirectory(src/services/mockservice)
