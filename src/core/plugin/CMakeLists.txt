set(CWD ${CMAKE_SOURCE_DIR}/src/core/plugin)
set(PLUGIN_SOURCES ${CWD}/plugin.cpp PARENT_SCOPE)
set(PLUGIN_TEST_SOURCES ${CWD}/plugin_test.cpp ${CWD}/pluginrunaction_test.cpp ${CWD}/pluginloader_test.cpp PARENT_SCOPE)