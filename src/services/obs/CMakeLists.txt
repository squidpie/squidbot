add_library(
    obs
    SHARED
    obs.cpp
    ${CMAKE_SOURCE_DIR}/src/events/event_client.cpp
    ${CMAKE_SOURCE_DIR}/src/events/event_server.cpp)

target_include_directories(
    obs
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/services
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/src/events)

target_include_directories(
    obs
    PUBLIC
    ${EXTERNAL_LIB_DIR}/plog/include)