# CMakeLists.txt
# Copyright (C) 2023  Squidpie

set(CWD ${CMAKE_SOURCE_DIR}/src/core/plugin)
set(PLUGIN_SOURCES ${CWD}/plugin.cpp ${CWD}/pluginmanager.cpp PARENT_SCOPE)
set(PLUGIN_TEST_SOURCES ${CWD}/plugin_test.cpp ${CWD}/pluginrunaction_test.cpp ${CWD}/pluginloader_test.cpp ${CWD}/pluginmanager_test.cpp PARENT_SCOPE)