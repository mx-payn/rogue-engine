#include "rogue/core/utils/Logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>
#include <unordered_map>

namespace Rogue {

  std::shared_ptr<spdlog::logger> Logger::s_Logger = nullptr;

  std::unordered_map<LogLevel, spdlog::level::level_enum> m_LevelMap
      = {{LogLevel::None, spdlog::level::off},   {LogLevel::Fatal, spdlog::level::critical},
         {LogLevel::Error, spdlog::level::err},  {LogLevel::Warn, spdlog::level::warn},
         {LogLevel::Info, spdlog::level::info},  {LogLevel::Debug, spdlog::level::debug},
         {LogLevel::Trace, spdlog::level::trace}};

  bool Logger::Init(LogLevel logLevel) {
    std::vector<spdlog::sink_ptr> logSinks;

    // stdout color sink
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    if (!logSinks[0]) return false;
    logSinks[0]->set_pattern("[%T] [%^%l%$] %v");

    // basic file sink
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("rogue.log", true));
    if (!logSinks[1]) return false;
    logSinks[1]->set_pattern("[%T] [%l] %v");

    // logger initialization
    s_Logger = std::make_shared<spdlog::logger>("Rogue", begin(logSinks), end(logSinks));
    if (!s_Logger) return false;
    spdlog::register_logger(s_Logger);
    s_Logger->set_level(m_LevelMap[logLevel]);
    s_Logger->flush_on(m_LevelMap[logLevel]);

    return true;
  }

  void Logger::SetLogLevel(LogLevel logLevel) { s_Logger->set_level(m_LevelMap[logLevel]); }

}  // namespace Rogue
