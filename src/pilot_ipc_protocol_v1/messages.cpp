#include "messages.hpp"

namespace sabre_pilot::ipc
{
    PilotIpcMessage::PilotIpcMessage(uint16_t type) : _type(type) {}

    ClientHelloMessage::ClientHelloMessage() : PilotIpcMessage(0) {}

    void ClientHelloMessage::parseBytes(std::vector<uint8_t> bytes) {}

    ServerHelloMessage::ServerHelloMessage() : PilotIpcMessage(0) {}

    void ServerHelloMessage::parseBytes(std::vector<uint8_t> bytes) {}
} // namespace sabre_pilot::ipc