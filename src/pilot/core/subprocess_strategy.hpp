#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace sabre_pilot
{
    class SubprocessStrategy
    {
    public:
        virtual ~SubprocessStrategy() = default;
        virtual uint32_t start(const std::string &program,
                               std::vector<std::string> args) const = 0;
        virtual void stop(uint32_t pid) const = 0;
        virtual bool isRunning(uint32_t pid) const = 0;
    };
} // namespace sabre_pilot