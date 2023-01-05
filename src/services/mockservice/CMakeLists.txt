add_library(
    mockserviceA
    SHARED
    mockservice.cpp)

target_link_libraries(
    mockserviceA
    PUBLIC
    LIB_FRAMEWORK)

set_target_properties(
    mockserviceA
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/test"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/test"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test")

add_library(
    mockserviceB
    SHARED
    mockservice.cpp)

target_link_libraries(
    mockserviceB
    PUBLIC
    LIB_FRAMEWORK)

set_target_properties(
    mockserviceB
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/test"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/test"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test")