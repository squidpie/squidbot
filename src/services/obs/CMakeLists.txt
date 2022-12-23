add_library(
    obs
    SHARED
    obs.cpp)

target_include_directories(
    obs
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/services)