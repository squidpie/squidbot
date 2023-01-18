set(CWD ${CMAKE_SOURCE_DIR}/src/core/events)
set(EVENTS_SOURCES ${CWD}/event_server.cpp ${CWD}/event_client.cpp PARENT_SCOPE)
set(EVENTS_TEST_SOURCES ${CWD}/event_server_test.cpp ${CWD}/event_client_test.cpp ${CWD}/events_test.cpp PARENT_SCOPE)