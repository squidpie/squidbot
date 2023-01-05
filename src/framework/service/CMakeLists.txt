set(CWD ${CMAKE_SOURCE_DIR}/src/framework/service)
set(SERVICE_SOURCES ${CWD}/service_manager.cpp PARENT_SCOPE)
set(SERVICE_TEST_SOURCES ${CWD}/service_test.cpp ${CWD}/service_manager_test.cpp ${CWD}/service_loader_test.cpp PARENT_SCOPE)