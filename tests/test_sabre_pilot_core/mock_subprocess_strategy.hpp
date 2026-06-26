#pragma once

#include <pilot_core/subprocess_strategy.hpp>

class MockSubprocessStrategy : public sabre_pilot::SubprocessStrategy
{
public:
    uint32_t start(const std::string &program,
                   std::vector<std::string> args) const override
    {
        return 1;
    }

    void stop(uint32_t pid) const override {}

    bool isRunning(uint32_t pid) const override
    {
        return true;
    }
};