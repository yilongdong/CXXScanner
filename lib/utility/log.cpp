#include "utility/log.h"
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <iostream>
#include <unordered_map>

tudumper::utility::MMLogger::MMLogger(tudumper::utility::LazySingleton<MMLogger>::token) {
    try {
        const std::string loggerName = std::string("test");
        if (m_console) {
            m_logger = spdlog::stderr_color_mt(loggerName);
        } else {
            m_logDir = std::filesystem::directory_entry(std::filesystem::current_path());
            auto logDirPath = m_logDir.path();
            // multipart log files, with every part 50M, max 100 files
            m_logger = spdlog::create_async<spdlog::sinks::rotating_file_sink_mt>(
                    loggerName, logDirPath.append(loggerName + ".log").u8string(),
                    50 * 1024 * 1024, 100);
        }

        setLogLevel(m_level);

        m_logger->set_pattern("[T:%-6t] %+");
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "[Logger] Initialization failed: " << ex.what() << '\n';
    }
}

void tudumper::utility::MMLogger::setLogLevel(std::string const& logLevel) {
    m_level = logLevel;
    static std::unordered_map<std::string, decltype(spdlog::level::trace)> levelConvertMap {
       { "trace", spdlog::level::trace },
       { "debug", spdlog::level::debug },
       { "info", spdlog::level::info },
       { "warn", spdlog::level::warn },
       { "error", spdlog::level::err },
       { "critical", spdlog::level::critical },
    };
    try {
        auto spdlogLevel = levelConvertMap.at(m_level);
        m_logger->set_level(spdlogLevel);
        m_logger->flush_on(spdlogLevel);
    }
    catch (...) {
        LOG_WARN("unknown log level");
    }
}

std::shared_ptr<spdlog::logger> tudumper::utility::MMLogger::getLogger() {
    return m_logger;
}

