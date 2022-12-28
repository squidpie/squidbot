#include <gtest/gtest.h>

#include "events.h"
#include "service.h"

const std::string TEST_LIB_PATH = "/home/squidpie/Repos/squidbot/targets/lib/test";

class ServiceTest: public testing::Test {
  protected:
    ServiceContainer_t * container_dut;
    ServiceContainer_t * error_dut;
    EventServer mock_event_server;

    void SetUp() {
      plog::init(plog::debug, "mock.log");
      Context_t context = {plog::get(), &mock_event_server};

      container_dut = new ServiceContainer("mockservice", &context);
      error_dut = new ServiceContainer("no_lib_found_error", &context);

      container_dut->gen_lib_path(TEST_LIB_PATH);
      ASSERT_NO_THROW(container_dut->init());
      ASSERT_NO_THROW(container_dut->create());
    }
};

TEST_F(ServiceTest, ServiceContainer_create) {}

TEST_F(ServiceTest, ServiceContainer_run) {
  ASSERT_NO_THROW(container_dut->run());
}

TEST_F(ServiceTest, ServiceContainer_destroy) {
  ASSERT_NO_THROW(container_dut->destroy());
  ASSERT_THROW(container_dut->run(), ServiceRuntimeError);
}

TEST_F(ServiceTest, ServiceContainer_throw) {
  error_dut->gen_lib_path(TEST_LIB_PATH);
  ASSERT_THROW(error_dut->init(), ServiceLibError);
}