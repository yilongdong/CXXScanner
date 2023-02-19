#pragma once
#include <filesystem>
#include <spdlog/spdlog.h>
#include "utility/singleton.h"

namespace tudumper::utility {
class MMLogger final : public tudumper::utility::LazySingleton<MMLogger> {
public:
    // 使用token防止直接构造对象
    MMLogger() = delete;
    explicit MMLogger(typename tudumper::utility::LazySingleton<MMLogger>::token);


    void setLogLevel(std::string const& level);
    std::shared_ptr<spdlog::logger> getLogger();

private:
    std::shared_ptr<spdlog::logger> m_logger;
    std::filesystem::directory_entry m_logDir;
    bool m_console{ true };
    std::string m_level{ "debug" };
};
}

#define SET_LOGLEVEL(level) tudumper::utility::MMLogger::instance().setLogLevel(level);
#define LOG_TRACE(...)      SPDLOG_LOGGER_CALL(tudumper::utility::MMLogger::instance().getLogger().get(), spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(...)      SPDLOG_LOGGER_CALL(tudumper::utility::MMLogger::instance().getLogger().get(), spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...)       SPDLOG_LOGGER_CALL(tudumper::utility::MMLogger::instance().getLogger().get(), spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(...)       SPDLOG_LOGGER_CALL(tudumper::utility::MMLogger::instance().getLogger().get(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...)      SPDLOG_LOGGER_CALL(tudumper::utility::MMLogger::instance().getLogger().get(), spdlog::level::err, __VA_ARGS__)
#define LOG_CRITICAL(...)   SPDLOG_LOGGER_CALL(tudumper::utility::MMLogger::instance().getLogger().get(), spdlog::level::critical, __VA_ARGS__)
