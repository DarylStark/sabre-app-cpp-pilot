#pragma once

#include <pilot_ipc_protocol/ipc_message.hpp>

namespace sabre_pilot::ipc
{
    class PilotIpcMessage : public IpcMessage
    {
    private:
        uint16_t _type;

    public:
        PilotIpcMessage(uint16_t type);
    };

    class ClientHelloMessage : public PilotIpcMessage
    {
    private:
        uint32_t _clientId = 0;

    public:
        ClientHelloMessage();
        void parseBytes(std::vector<uint8_t> data) override;
    };

    class ServerHelloMessage : public PilotIpcMessage
    {
    private:
        uint32_t _clientId = 0;

    public:
        ServerHelloMessage();
        void parseBytes(std::vector<uint8_t> data) override;
    };
} // namespace sabre_pilot::ipc