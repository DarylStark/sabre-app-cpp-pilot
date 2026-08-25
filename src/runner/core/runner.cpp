#include "runner.hpp"
#include <iostream>

namespace sabre_runner::core
{
    Runner::Runner(CoreConfig config) : _config(std::move(config)) {}

    void Runner::start()
    {
        std::cout << "Runner starting!\n";
        std::cout << _config.software.firmwareFile << "\n";
    }
} // namespace sabre_runner::core