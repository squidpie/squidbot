# CMakeLists.txt
# Copyright (C) 2023  Squidpie

add_library(
  adminlisten
  SHARED
  adminlisten.cpp factory.cpp)

target_link_libraries(
  adminlisten
  PUBLIC
  LIB_CORE)

set_target_properties(
  adminlisten
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/services")

set(ADMIN_LISTEN_SOURCE ${CMAKE_SOURCE_DIR}/src/services/adminlisten/adminlisten.cpp PARENT_SCOPE)
