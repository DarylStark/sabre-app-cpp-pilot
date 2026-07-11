#include "message_factory.hpp"

namespace sabre_pilot::ipc
{
    std::shared_ptr<PilotIpcMessage>
    MessageFactory::createMessage(uint16_t type)
    {
        if (type == 0x00)
            return createClientHello();
        if (type == 0x01)
            return createServerHello();

        throw std::runtime_error("Uknown message!"); // TODO: Custom exception
    }

    std::shared_ptr<ClientHelloMessage> MessageFactory::createClientHello()
    {
        return std::make_shared<ClientHelloMessage>();
    }

    std::shared_ptr<ServerHelloMessage> MessageFactory::createServerHello()
    {
        return std::make_shared<ServerHelloMessage>();
    }
} // namespace sabre_pilot::ipc