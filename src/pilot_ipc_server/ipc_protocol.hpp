#pragma once

#include <cstdint>
#include <vector>

namespace sabre_pilot
{
    class IpcProtocol
    {
    protected:
        std::vector<uint8_t> _dataBuffer;

    public:
        virtual ~IpcProtocol() = default;
        void addData(const std::vector<uint8_t> &data);
        virtual void processData() = 0;
    };
} // namespace sabre_pilot