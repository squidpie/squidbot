/*
gtest_main.cpp
Copyright (C) 2023  Squidpie
 */

#include <gtest/gtest.h>

#include "logging.h"
#include "utils/defines.h"

int main(int argc, char **argv) {
  plog::init(plog::debug, "mock.log");
  plog_shared_init(plog::debug, plog::get(), LIB_DIR);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}