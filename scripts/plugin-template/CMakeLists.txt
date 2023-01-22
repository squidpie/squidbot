# CMakeLists.txt
# Copyright (C) 2023  Squidpie

add_library(
    templatename
    SHARED
    templatename.cpp factory.cpp ${MOCK_SERVICE_SOURCES})

target_link_libraries(
    templatename
    PUBLIC
    LIB_CORE)

set_target_properties(
    templatename
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/plugins"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins")

set(TEMPLATENAME_SOURCES ${CMAKE_SOURCE_DIR}/src/plugins/templatename/templatename.cpp PARENT_SCOPE)