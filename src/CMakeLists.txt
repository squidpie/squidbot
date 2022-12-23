# Main Build settings
set(SOURCE_FILES
    squidbot.cpp services/service_manager.cpp services/obs/obs.cpp services/service.cpp)

add_executable(
    squidbot
    ${SOURCE_FILES})

target_include_directories(
    squidbot
    PRIVATE
    .
    services
    services/obs)

target_link_libraries(
    squidbot
    PRIVATE
    ${CMAKE_DL_LIBS})