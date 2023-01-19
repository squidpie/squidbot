# Main Build settings

set(SOURCE_FILES squidbot.cpp logging/logging.cpp)

add_executable(
    squidbot
    ${SOURCE_FILES})

target_link_libraries(
    squidbot
    PUBLIC
    LIB_CORE)

target_link_libraries(
    squidbot
    PRIVATE
    ${CMAKE_DL_LIBS}
    Threads::Threads)