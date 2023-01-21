# CMakeLists.txt
# Copyright (C) 2023  Squidpie

add_library(
    mockservice
    SHARED
    mockservice.cpp factory.cpp)

target_link_libraries(
    mockservice
    PUBLIC
    LIB_CORE)

set_target_properties(
    mockservice
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/services"
    EXCLUDE_FROM_ALL True)

set(MOCK_SERVICE_SOURCES ${CMAKE_SOURCE_DIR}/src/services/mockservice/mockservice.cpp PARENT_SCOPE)