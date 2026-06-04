#include <gtest/gtest.h>
#include <pilot_core/dynamic_library.hpp>
#include <pilot_core/exceptions.hpp>

class MockDynamicLoader : public sabre_pilot::DynamicLibrary
{
public:
    MockDynamicLoader(const std::string &libLocation)
        : sabre_pilot::DynamicLibrary(libLocation)
    {
    }

    void load() override {}
};

TEST(DynamicLibraryTests, AddingAndRetrievingEntryPoint)
{
    MockDynamicLoader mock("test.lib");
    mock.addEntryPoint("test");
    ASSERT_EQ(mock.getEntryPoint("test"), nullptr);
}

TEST(DynamicLibraryTests, RetrievingNonExistingEntryPoint)
{
    MockDynamicLoader mock("test.lib");
    ASSERT_THROW(mock.getEntryPoint("test"),
                 sabre_pilot::NonExistingEntryPointException);
}