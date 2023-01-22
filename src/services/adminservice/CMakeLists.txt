# CMakeLists.txt
# Copyright (C) 2023  Squidpie

add_library(
  adminservice
  SHARED
  adminservice.cpp factory.cpp)

target_link_libraries(
  adminservice
  PUBLIC
  LIB_CORE)

set_target_properties(
  adminservice
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/services")

set(ADMIN_SERVICE_SOURCE ${CMAKE_SOURCE_DIR}/src/services/adminservice/adminservice.cpp PARENT_SCOPE)
