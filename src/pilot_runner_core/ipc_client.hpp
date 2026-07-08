#pragma once

#include <string>

namespace sabre_pilot_runner_core
{
    class IpcClient
    {
    public:
        virtual ~IpcClient() = default;
        virtual void connect() = 0;
        virtual void disconnect() = 0;
        virtual void sendData(const std::string &data) = 0;
    };
} // namespace sabre_pilot_runner_core