#pragma once
#include <CLI/CLI.hpp>
#include <core/config.hpp>
#include <core/runner.hpp>

namespace sabre_pilot::runner
{
    class RunnerStarter
    {
    private:
        int _argc;
        char **_argv;

        CLI::App _app;

        sabre_runner::core::CoreConfig _config;
        std::string _ipcMode;
        sabre_runner::core::IpcTcpConfig _ipc_tcp_config;

        void _addPositionalArgs();
        void _addOptionalHardwareArgs();
        void _addOptionalSoftwareArgs();
        void _addOptionalIpcArgs();
        int _parse();
        void _startRunner();

    public:
        RunnerStarter(int argc, char *argv[]) noexcept;
        int main();
    };
} // namespace sabre_pilot::runner