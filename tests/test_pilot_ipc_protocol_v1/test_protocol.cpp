#include <gtest/gtest.h>
#include <pilot_ipc_protocol_v1/ipc_protocol.hpp>

using namespace sabre_pilot::ipc;

TEST(IpcProtocol_v1, ClientHelloMessage)
{
    std::queue<std::shared_ptr<sabre_pilot::ipc::IpcMessage>> queue;
    PilotIpcProtocol_v1 protocol(queue);

    protocol.addData({0x00, 0x00, 0x00, 0x04, 0x0a, 0x0b, 0x0c, 0x0d});

    ASSERT_EQ(queue.size(), 1);

    auto lastElement = queue.front();
    queue.pop();

    auto clientHello =
        std::dynamic_pointer_cast<ClientHelloMessage>(lastElement);
    ASSERT_NE(clientHello, nullptr);
}