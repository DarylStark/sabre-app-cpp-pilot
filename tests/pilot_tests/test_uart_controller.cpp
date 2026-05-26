#include <gtest/gtest.h>
#include <pilot/exceptions.hpp>
#include <pilot/uart_controller.hpp>

TEST(UartBuffer, WriteOneByteToBuffer)
{
    sabre_pilot::UartController buffer;
    buffer.initialize(1, 4);
    buffer.write('t');
    ASSERT_EQ(buffer.getOutputBuffer(), "t");
}

TEST(UartBuffer, WriteTwoBytesToBuffer)
{
    sabre_pilot::UartController buffer;
    buffer.initialize(1, 4);
    buffer.write('t');
    buffer.write('e');
    ASSERT_EQ(buffer.getOutputBuffer(), "te");
}

TEST(UartBuffer, WriteFourBytesToBuffer)
{
    sabre_pilot::UartController buffer;
    buffer.initialize(1, 4);
    buffer.write('t');
    buffer.write('e');
    buffer.write('s');
    buffer.write('t');
    ASSERT_EQ(buffer.getOutputBuffer(), "");
}

TEST(UartBuffer, OverFlowBuffer)
{
    sabre_pilot::UartController buffer;
    buffer.initialize(1, 4);
    buffer.write('t');
    buffer.write('e');
    buffer.write('s');
    buffer.write('t');
    buffer.write('t');
    buffer.write('e');
    buffer.write('s');
    ASSERT_EQ(buffer.getOutputBuffer(), "tes");
}

TEST(UartBuffer, FlushBuffer)
{
    sabre_pilot::UartController buffer;
    buffer.initialize(1, 4);
    buffer.write('t');
    buffer.flush();
    ASSERT_EQ(buffer.getOutputBuffer(), "");
}

TEST(UartBuffer, OutputBufferCallbackCalledAutomatically)
{
    sabre_pilot::UartController buffer;
    buffer.initialize(1, 4);
    std::string outputText;
    buffer.setOutputBufferCallback([&outputText](char c)
                                   { outputText.push_back(c); });
    buffer.write('t');
    buffer.write('e');
    buffer.write('s');
    buffer.write('t');
    ASSERT_EQ(outputText, "test");
}

TEST(UartBuffer, ExceptionOnWritingToUnitializedUart)
{
    sabre_pilot::UartController buffer;
    ASSERT_THROW(
        buffer.write('b'),
        sabre_pilot::exceptions::UartControllerNotInitializedException);
}

TEST(UartBuffer, ExceptionOnFlushingUnitializedUart)
{
    sabre_pilot::UartController buffer;
    ASSERT_THROW(
        buffer.flush(),
        sabre_pilot::exceptions::UartControllerNotInitializedException);
}