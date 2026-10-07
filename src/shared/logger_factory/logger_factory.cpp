#include "logger_factory.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace sabre_logger_factory
{
    LoggerFactory::LoggerFactory()
    {
        _sinks.push_back(
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        _sinks.front()->set_level(spdlog::level::info);
        _sinks.front()->set_pattern("[%H:%M:%S] [%^%l%$] [%n] %v");
    }

    std::shared_ptr<spdlog::logger> LoggerFactory::make(const std::string &name)
    {
        auto logger =
            std::make_shared<spdlog::logger>(name, begin(_sinks), end(_sinks));
        logger->set_level(spdlog::level::info);
        spdlog::register_logger(logger);
        return logger;
    }
} // namespace sabre_logger_factory