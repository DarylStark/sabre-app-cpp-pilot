#include <gtest/gtest.h>
#include <sabre_impl/exceptions.hpp>
#include <sabre_impl/uart_controller.hpp>

class UartControllerTest : public ::testing::Test
{
public:
    UartControllerTest() {}

protected:
    sabre::impl::pilot::UartController _buffer;
};

TEST_F(UartControllerTest, WriteOneByteToBuffer)
{
    _buffer.initialize(1, 4);
    _buffer.write('t');
    ASSERT_EQ(_buffer.getOutputBuffer(), "t");
}

TEST_F(UartControllerTest, WriteTwoBytesToBuffer)
{
    _buffer.initialize(1, 4);
    _buffer.write('t');
    _buffer.write('e');
    ASSERT_EQ(_buffer.getOutputBuffer(), "te");
}

TEST_F(UartControllerTest, WriteFourBytesToBuffer)
{
    _buffer.initialize(1, 4);
    _buffer.write('t');
    _buffer.write('e');
    _buffer.write('s');
    _buffer.write('t');
    ASSERT_EQ(_buffer.getOutputBuffer(), "");
}

TEST_F(UartControllerTest, OverFlowBuffer)
{
    _buffer.initialize(1, 4);
    _buffer.write('t');
    _buffer.write('e');
    _buffer.write('s');
    _buffer.write('t');
    _buffer.write('t');
    _buffer.write('e');
    _buffer.write('s');
    ASSERT_EQ(_buffer.getOutputBuffer(), "tes");
}

TEST_F(UartControllerTest, FlushBuffer)
{
    _buffer.initialize(1, 4);
    _buffer.write('t');
    _buffer.flush();
    ASSERT_EQ(_buffer.getOutputBuffer(), "");
}

TEST_F(UartControllerTest, OutputBufferCallbackCalledAutomatically)
{
    _buffer.initialize(1, 4);
    std::string outputText;
    _buffer.setOutputBufferCallback([&outputText](char c)
                                    { outputText.push_back(c); });
    _buffer.write('t');
    _buffer.write('e');
    _buffer.write('s');
    _buffer.write('t');
    ASSERT_EQ(outputText, "test");
}

TEST_F(UartControllerTest, ExceptionOnWritingToUnitializedUart)
{
    ASSERT_THROW(_buffer.write('b'),
                 sabre::impl::pilot::UartControllerNotInitializedException);
}

TEST_F(UartControllerTest, ExceptionOnFlushingUnitializedUart)
{
    ASSERT_THROW(_buffer.flush(),
                 sabre::impl::pilot::UartControllerNotInitializedException);
}

TEST_F(UartControllerTest, IsNotInitializedAfterCreation)
{
    ASSERT_FALSE(_buffer.isInitialized());
}

TEST_F(UartControllerTest, Initialize)
{
    _buffer.initialize(1, 1);
    ASSERT_TRUE(_buffer.isInitialized());
}

TEST_F(UartControllerTest, InitializeDeinitialize)
{
    _buffer.initialize(1, 1);
    _buffer.deinitialize();
    ASSERT_FALSE(_buffer.isInitialized());
}