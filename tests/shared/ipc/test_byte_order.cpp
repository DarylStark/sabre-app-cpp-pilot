#include <gtest/gtest.h>
#include <ipc/byte_order.hpp>
#include <string>

using namespace ipc::byte_order;

TEST(IpcByteOrder, SerializeUnsignedInteger)
{
    unsigned int input = 0xff123456;
    auto result = serialize(input);

    ASSERT_EQ(result.size(), 4);
    ASSERT_EQ(result[0], std::byte{0xff});
    ASSERT_EQ(result[1], std::byte{0x12});
    ASSERT_EQ(result[2], std::byte{0x34});
    ASSERT_EQ(result[3], std::byte{0x56});
}

TEST(IpcByteOrder, SerializeSignedInteger)
{
    int input = -500;
    auto result = serialize(input);

    ASSERT_EQ(result.size(), 4);
    ASSERT_EQ(result[0], std::byte{0xff});
    ASSERT_EQ(result[1], std::byte{0xff});
    ASSERT_EQ(result[2], std::byte{0xfe});
    ASSERT_EQ(result[3], std::byte{0x0c});
}

TEST(IpcByteOrder, SerializeFloat)
{
    float input = 20.012022f;
    auto result = serialize(input);

    ASSERT_EQ(result.size(), 4);
    ASSERT_EQ(result[0], std::byte{0x41});
    ASSERT_EQ(result[1], std::byte{0xa0});
    ASSERT_EQ(result[2], std::byte{0x18});
    ASSERT_EQ(result[3], std::byte{0x9f});
}

TEST(IpcByteOrder, SerializeDouble)
{
    double input = 20.012022012022012;
    auto result = serialize(input);

    ASSERT_EQ(result.size(), 8);
    ASSERT_EQ(result[0], std::byte{0x40});
    ASSERT_EQ(result[1], std::byte{0x34});
    ASSERT_EQ(result[2], std::byte{0x03});
    ASSERT_EQ(result[3], std::byte{0x13});
    ASSERT_EQ(result[4], std::byte{0xdf});
    ASSERT_EQ(result[5], std::byte{0xe4});
    ASSERT_EQ(result[6], std::byte{0x77});
    ASSERT_EQ(result[7], std::byte{0x77});
}

TEST(IpcByteOrder, SerializeString)
{
    std::string input = "Grace Hopper";
    auto result = serialize(input);
    ASSERT_EQ(result.size(), input.size());

    ASSERT_EQ(result[0], std::byte{'G'});
    ASSERT_EQ(result[1], std::byte{'r'});
    ASSERT_EQ(result[2], std::byte{'a'});
    ASSERT_EQ(result[3], std::byte{'c'});
    ASSERT_EQ(result[4], std::byte{'e'});
    ASSERT_EQ(result[5], std::byte{' '});
    ASSERT_EQ(result[6], std::byte{'H'});
    ASSERT_EQ(result[7], std::byte{'o'});
    ASSERT_EQ(result[8], std::byte{'p'});
    ASSERT_EQ(result[9], std::byte{'p'});
    ASSERT_EQ(result[10], std::byte{'e'});
    ASSERT_EQ(result[11], std::byte{'r'});
}

TEST(IpcByteOrder, DeserializeUnsignedInteger)
{
    uint32_t result = deserialize<uint32_t>(std::vector{
        std::byte{0xff}, std::byte{0x12}, std::byte{0x34}, std::byte{0x56}});

    ASSERT_EQ(result, 0xff123456);
}

TEST(IpcByteOrder, DeserializeSignedInteger)
{
    int32_t result = deserialize<int32_t>(std::vector{
        std::byte{0xff}, std::byte{0xff}, std::byte{0xfe}, std::byte{0x0c}});

    ASSERT_EQ(result, -500);
}

TEST(IpcByteOrder, DeserializeFloat)
{
    float result = deserialize<float>(std::vector{
        std::byte{0x41}, std::byte{0xa0}, std::byte{0x18}, std::byte{0x9f}});

    ASSERT_NEAR(result, 20.012022, 0.0000001);
}

TEST(IpcByteOrder, DeserializeDouble)
{
    double result = deserialize<double>(std::vector{
        std::byte{0x40}, std::byte{0x34}, std::byte{0x03}, std::byte{0x013},
        std::byte{0xdf}, std::byte{0xe4}, std::byte{0x77}, std::byte{0x77}});

    ASSERT_EQ(result, 20.012022012022012);
}

TEST(IpcByteOrder, DeserializeString)
{
    std::string result =
        deserializeString({std::byte{'G'}, std::byte{'r'}, std::byte{'a'},
                           std::byte{'c'}, std::byte{'e'}});
    ASSERT_EQ(result, "Grace");
}

TEST(IpcByteOrder, SerializeAndDeserializeUnsignedInteger)
{
    uint32_t nr = deserialize<uint32_t>(serialize<uint32_t>(2610));
    ASSERT_EQ(nr, 2610);
}

TEST(IpcByteOrder, SerializeAndDeserializeSignedInteger)
{
    int32_t nr = deserialize<int32_t>(serialize<int32_t>(-2512));
    ASSERT_EQ(nr, -2512);
}

TEST(IpcByteOrder, SerializeAndDeserializeFloat)
{
    float nr = deserialize<float>(serialize<float>(33.01f));
    ASSERT_EQ(nr, 33.01f);
}

TEST(IpcByteOrder, SerializeAndDeserializeDouble)
{
    double nr = deserialize<double>(serialize<double>(33.01));
    ASSERT_EQ(nr, 33.01);
}

TEST(IpcByteOrder, SerializeAndDeserializeString)
{
    std::string str = deserializeString(serialize(std::string("Brachypelma")));
    ASSERT_EQ(str, "Brachypelma");
}