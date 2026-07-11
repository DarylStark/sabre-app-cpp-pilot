#pragma once

#include <cstdint>
#include <vector>

namespace sabre_pilot
{
    class IpcProtocol
    {
    protected:
        std::vector<uint8_t> _frameBuffer;

    public:
        void addData(std::vector<uint8_t>);
        virtual void processData() = 0;
    };
} // namespace sabre_pilot