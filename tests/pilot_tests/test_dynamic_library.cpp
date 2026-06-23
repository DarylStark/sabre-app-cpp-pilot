#include <gtest/gtest.h>
#include <pilot_core/dynamic_library.hpp>
#include <pilot_core/exceptions.hpp>

class MockDynamicLoader : public sabre_pilot::DynamicLibrary
{
private:
    void _loadEntryPoint(const std::string &entryPoint) override {}
    void _loadLibrary() override {}

public:
    MockDynamicLoader(const std::string &libLocation)
        : sabre_pilot::DynamicLibrary(libLocation)
    {
    }
};
