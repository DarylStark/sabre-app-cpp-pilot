#include <gtest/gtest.h>
#include <sabre_impl/core.hpp>

class CoreFactory : public ::testing::Test
{
public:
    CoreFactory()
    {
        sabre::core::ResourceManagerConfig config = {.maxGpios = 30,
                                                     .upperboundUart = 3};
        _factory = std::make_unique<sabre::impl::pilot::Factory>();
    }

protected:
    sabre::core::Factory::UniquePtr _factory;
};

TEST_F(CoreFactory, CreateUsbCdc)
{
    ASSERT_EQ(_factory->createUsbCdc(0, 100), nullptr);
}

TEST_F(CoreFactory, CreateUartObject)
{
    ASSERT_NE(_factory->createUartObject(0, 9600, 1, 2, 0), nullptr);
}

TEST_F(CoreFactory, CreateInputGpio)
{
    ASSERT_EQ(_factory->createInputGpio(1), nullptr);
}

TEST_F(CoreFactory, CreateOutputGpio)
{
    ASSERT_EQ(_factory->createOutputGpio(1), nullptr);
}

TEST_F(CoreFactory, CreateGpio)
{
    ASSERT_NE(_factory->createGpio(1), nullptr);
}

TEST_F(CoreFactory, CreateWifiStation)
{
    ASSERT_EQ(_factory->createWifiStation(), nullptr);
}

TEST_F(CoreFactory, CreateWifiSoftAp)
{
    ASSERT_EQ(_factory->createWifiSoftAp(), nullptr);
}

TEST_F(CoreFactory, CreateWallClock)
{
    ASSERT_EQ(_factory->createWallClock(), nullptr);
}

TEST_F(CoreFactory, CreateNtpClient)
{
    ASSERT_EQ(_factory->createNtpClient("ntp.test.org"), nullptr);
}

TEST_F(CoreFactory, CreateMqttClient)
{
    ASSERT_EQ(_factory->createMqttClient(), nullptr);
}

TEST_F(CoreFactory, CreateWaitFor)
{
    ASSERT_EQ(_factory->createWaitFor([]() { return true; }, 0, 10), nullptr);
}

TEST_F(CoreFactory, CreateService)
{
    ASSERT_EQ(_factory->createService([]() {}), nullptr);
}

TEST_F(CoreFactory, CreatePlatform)
{
    ASSERT_EQ(_factory->createPlatform(), nullptr);
}

TEST_F(CoreFactory, CreateQueue)
{
    ASSERT_EQ(_factory->createQueue(1, 10), nullptr);
}

TEST_F(CoreFactory, CreateHttpServer)
{
    ASSERT_EQ(_factory->createHttpServer(), nullptr);
}

TEST_F(CoreFactory, CreateRgbPixelStrip)
{
    ASSERT_EQ(_factory->createRgbPixelStrip(0, 10), nullptr);
}