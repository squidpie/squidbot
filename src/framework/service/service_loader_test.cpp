#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "events.h"
#include "logging.h"
#include "service_loader.h"
#include "servicelib.h"
#include "utils.h"

#include "service_test_mock.h"

using namespace testing;

class LibLoaderTest : public testing::Test {
protected:
  std::shared_ptr<ServiceTestServiceManager> service_manager;
  std::shared_ptr<ServiceTestEventServer> event_server;
  std::shared_ptr<ServiceTestEventClient> event_client;

  std::shared_ptr<ServiceContext> context;
  std::unique_ptr<LibLoader<ServiceLoader>> dut;

  std::vector<std::shared_ptr<ServiceBase>> services;

  void SetUp() override {}

  void TearDown() override {
    for(auto s : services) {
      if (s) s->stop();
      s.reset();
    }
  }
};

TEST_F(LibLoaderTest, setup) {
  event_server = std::make_shared<ServiceTestEventServer>();
  event_client = std::make_shared<ServiceTestEventClient>();
  context = std::make_shared<ServiceContext>(plog::get(), event_server,
                                             service_manager, TEST_LIB_DIR);
  service_manager = std::make_shared<ServiceTestServiceManager>(context);
  context->service_manager = service_manager;

  EXPECT_CALL(*event_server, create_client())
      .WillRepeatedly(testing::Return(event_client));
  EXPECT_CALL(*event_client, subscribe(testing::_)).WillRepeatedly(testing::Return());

  EXPECT_CALL(*service_manager, register_service(testing::_, testing::_))
      .WillRepeatedly(Invoke([this](std::type_index __i,
                          std::shared_ptr<ServiceBase> service) {
        services.push_back(service);
      }));

  EXPECT_CALL(*event_client, receive())
      .WillRepeatedly(testing::Return(NULL_EVENT));
  dut = std::make_unique<LibLoader<ServiceLoader>>(
      std::dynamic_pointer_cast<Context>(context));

  sleep(1);
}
