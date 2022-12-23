add_library(
    mockservice
    SHARED
    mockservice.cpp)

target_include_directories(
    mockservice
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/services)

set_target_properties(
    mockservice
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/test"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/test"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test")