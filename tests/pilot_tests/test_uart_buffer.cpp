#include <gtest/gtest.h>
#include <pilot/uart_buffer.hpp>

TEST(UartBuffer, AddAndRetrieveData)
{
    sabre_pilot::UartBuffer buffer;
    buffer.addData("test");
    ASSERT_EQ(buffer.getBuffer(), "test");
}
