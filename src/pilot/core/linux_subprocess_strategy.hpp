#include "subprocess_strategy.hpp"

namespace sabre_pilot::core
{
    class LinuxSubprocessStrategy : public SubprocessStrategy
    {
    public:
        uint32_t start(const std::string &program,
                       std::vector<std::string> args) const override;
        void stop(uint32_t pid) const override;
        bool isRunning(uint32_t pid) const override;
    };
} // namespace sabre_pilot::core