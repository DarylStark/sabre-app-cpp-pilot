#pragma once

namespace sabre_pilot
{
    class IpcServer
    {
    public:
        virtual ~IpcServer() = default;
        virtual void setup() = 0;
        virtual void start() = 0;
        virtual void stop() = 0;
    };
} // namespace sabre_pilot