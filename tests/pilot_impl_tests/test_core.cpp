#include <gtest/gtest.h>
#include <pilot_impl/core.hpp>

TEST(Core, FactoryCreateUsbCdc)
{
    sabre::impl::pilot::Factory fac(nullptr);
    ASSERT_EQ(fac.createUsbCdc(0, 100), nullptr);
}