#include <gtest/gtest.h>

#include "service.h"

const std::string TEST_LIB_PATH = "/home/squidpie/Repos/squidbot/targets/lib/test";

class ServiceTest: public testing::Test {
  protected:
    ServiceContainer_t container_dut = ServiceContainer("mockservice");
    ServiceContainer_t error_dut = ServiceContainer("no_lib_found_error");

    void SetUp() {
      container_dut.gen_lib_path(TEST_LIB_PATH);
      ASSERT_NO_THROW(container_dut.init());
      ASSERT_NO_THROW(container_dut.create());
    }
};

TEST_F(ServiceTest, ServiceContainer_create) {}

TEST_F(ServiceTest, ServiceContainer_run) {
  ASSERT_TRUE(container_dut.run());
}

TEST_F(ServiceTest, ServiceContainer_destroy) {
  ASSERT_NO_THROW(container_dut.destroy());
  ASSERT_THROW(container_dut.run(), ServiceRuntimeError);
}

TEST_F(ServiceTest, ServiceContainer_throw) {
  error_dut.gen_lib_path(TEST_LIB_PATH);
  ASSERT_THROW(error_dut.init(), ServiceLibError);
}