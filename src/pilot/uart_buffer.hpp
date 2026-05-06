#pragma once

#include <string>

namespace sabre_pilot
{
    class UartBuffer
    {
    private:
        std::string _buffer;

    public:
        UartBuffer();
        void addData(const std::string &data);
        const std::string &getBuffer() const;
    };
} // namespace sabre_pilot