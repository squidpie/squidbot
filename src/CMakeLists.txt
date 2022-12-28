# Main Build settings
set(SOURCE_FILES
    squidbot.cpp services/service_manager.cpp services/obs/obs.cpp
    services/service.cpp events/event_client.cpp
    events/event_server.cpp)

add_executable(
    squidbot
    ${SOURCE_FILES})

target_include_directories(
    squidbot
    PRIVATE
    .
    services
    services/obs
    events)

target_include_directories(
    squidbot
    PUBLIC
    ${EXTERNAL_LIB_DIR}/plog/include)

target_link_libraries(
    squidbot
    PRIVATE
    ${CMAKE_DL_LIBS}
    Threads::Threads)