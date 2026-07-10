#pragma once

#include <string>

namespace sabre_pilot_runner_core
{
    class IpcClient
    {
    public:
        virtual ~IpcClient() = default;
        virtual void setup() = 0;
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual bool waitForConnection() = 0;
        virtual void sendData(const std::string &data) = 0;
    };
} // namespace sabre_pilot_runner_core