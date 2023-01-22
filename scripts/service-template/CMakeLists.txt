# CMakeLists.txt
# Copyright (C) 2023  Squidpie

add_library(
  templatename
  SHARED
  templatename.cpp factory.cpp)

target_link_libraries(
  templatename
  PUBLIC
  LIB_CORE)

set_target_properties(
  templatename
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/services")

set(TEMPLATENAME_SOURCES ${CMAKE_SOURCE_DIR}/src/services/templatename/templatename.cpp PARENT_SCOPE)
