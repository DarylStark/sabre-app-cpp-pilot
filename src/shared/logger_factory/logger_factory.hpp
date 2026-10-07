#pragma once

#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <vector>

namespace sabre_logger_factory
{
    class LoggerFactory
    {
    private:
        std::vector<spdlog::sink_ptr> _sinks;

    public:
        LoggerFactory();
        std::shared_ptr<spdlog::logger> make(const std::string &name);
    };
} // namespace sabre_logger_factory