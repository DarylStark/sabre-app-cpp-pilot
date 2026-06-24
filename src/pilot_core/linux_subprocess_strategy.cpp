#include "linux_subprocess_strategy.hpp"
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <signal.h>
#include <spawn.h>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>

extern char **environ;

namespace sabre_pilot
{
    uint32_t LinuxSubprocessStrategy::start(const std::string &program,
                                            std::vector<std::string> args) const
    {
        std::vector<char *> argv;
        argv.reserve(args.size() + 2);

        argv.push_back(const_cast<char *>(program.c_str()));
        for (const auto &arg : args)
        {
            argv.push_back(const_cast<char *>(arg.c_str()));
        }
        argv.push_back(nullptr);

        pid_t pid = -1;
        int rc = posix_spawnp(&pid, program.c_str(), nullptr, nullptr,
                              argv.data(), environ);

        if (rc != 0)
        {
            // TODO: Custom exception
            throw std::runtime_error("posix_spawnp failed: " +
                                     std::string(std::strerror(rc)));
        }

        return static_cast<uint32_t>(pid);
    }

    void LinuxSubprocessStrategy::stop(uint32_t pid) const
    {
        if (kill(pid, SIGTERM) == -1)
        {
            // TODO: Custom exception
            throw std::runtime_error("kill(SIGTERM) failed: " +
                                     std::string(std::strerror(errno)));
        }
    }
} // namespace sabre_pilot