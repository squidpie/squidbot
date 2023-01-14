add_library(
    admin
    SHARED
    admin.cpp factory.cpp ${MOCK_SERVICE_SOURCES})

target_link_libraries(
    admin
    PUBLIC
    LIB_CORE)

set_target_properties(
    admin
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/plugins"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins")

set(ADMIN_SOURCES ${CMAKE_SOURCE_DIR}/src/plugins/admin/admin.cpp PARENT_SCOPE)