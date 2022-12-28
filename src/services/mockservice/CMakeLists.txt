add_library(
    mockservice
    SHARED
    mockservice.cpp
    ${CMAKE_SOURCE_DIR}/src/events/event_server.cpp
    ${CMAKE_SOURCE_DIR}/src/events/event_client.cpp)

target_include_directories(
    mockservice
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/services
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/events
    ${EXTERNAL_LIB_DIR}/plog/include)

set_target_properties(
    mockservice
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/test"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/test"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test")