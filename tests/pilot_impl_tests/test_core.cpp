#include <gtest/gtest.h>
#include <pilot_impl/core.hpp>

TEST(Core, FactoryCreateUsbCdc)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createUsbCdc(0, 100), nullptr);
}

TEST(Core, FactoryCreateUartObject)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createUartObject(0, 9600, 1, 2, 0), nullptr);
}

TEST(Core, FactoryCreateInputGpio)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createInputGpio(1), nullptr);
}

TEST(Core, FactoryCreateOutputGpio)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createOutputGpio(1), nullptr);
}

TEST(Core, FactoryCreateGpio)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createGpio(1), nullptr);
}

TEST(Core, FactoryCreateWifiStation)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createWifiStation(), nullptr);
}

TEST(Core, FactoryCreateWifiSoftAp)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createWifiSoftAp(), nullptr);
}

TEST(Core, FactoryCreateWallClock)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createWallClock(), nullptr);
}

TEST(Core, FactoryCreateNtpClient)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createNtpClient("ntp.test.org"), nullptr);
}

TEST(Core, FactoryCreateMqttClient)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createMqttClient(), nullptr);
}

TEST(Core, FactoryCreateWaitFor)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createWaitFor([]() { return true; }, 0, 10), nullptr);
}

TEST(Core, FactoryCreateService)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createService([]() {}), nullptr);
}

TEST(Core, FactoryCreatePlatform)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createPlatform(), nullptr);
}

TEST(Core, FactoryCreateQueue)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createQueue(1, 10), nullptr);
}

TEST(Core, FactoryCreateHttpServer)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createHttpServer(), nullptr);
}

TEST(Core, FactoryCreateRgbPixelStrip)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createRgbPixelStrip(0, 10), nullptr);
}