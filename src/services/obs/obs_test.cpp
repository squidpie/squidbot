#include <gtest/gtest.h>

#include "obs.h"

class OBSTest: public testing::Test {
protected:
  OBS dut;
};

TEST_F(OBSTest, OBS_init) {
  ASSERT_NO_THROW(dut.load());
}