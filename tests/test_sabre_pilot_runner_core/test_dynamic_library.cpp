#include <gtest/gtest.h>
#include <pilot_runner_core/dynamic_library.hpp>

class MockDynamicLoader : public sabre_pilot_runner_core::DynamicLibrary
{
private:
    void _loadEntryPoint(const std::string &entryPoint) override {}
    void _loadLibrary() override {}

public:
    MockDynamicLoader(const std::string &libLocation)
        : sabre_pilot_runner_core::DynamicLibrary(libLocation)
    {
    }
};
