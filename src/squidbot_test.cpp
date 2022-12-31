#include <gtest/gtest.h>

#include <thread>
#include <unistd.h>

#include "squidbot.h"

class SquidbotTest: public testing::Test {
protected:
  void SetUp() override {
    plog::init(plog::debug, "mock.log");
  }

};

TEST_F(SquidbotTest, main) {
}