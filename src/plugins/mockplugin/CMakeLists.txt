# CMakeLists.txt
# Copyright (C) 2023  Squidpie

add_library(
    mockplugin
    SHARED
    mockplugin.cpp factory.cpp ${MOCK_SERVICE_SOURCES})

target_link_libraries(
    mockplugin
    PUBLIC
    LIB_CORE)

set_target_properties(
    mockplugin
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/plugins"
    EXCLUDE_FROM_ALL True)

set(MOCKPLUGIN_SOURCES ${CMAKE_SOURCE_DIR}/src/plugins/mockplugin/mockplugin.cpp PARENT_SCOPE)