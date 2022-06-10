#include "rogue/core/utils/Logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>

#include <memory>
#include <unordered_map>

namespace Rogue {

  std::shared_ptr<spdlog::logger> Logger::s_Logger = nullptr;
  static bool s_Initialized = false;

  std::unordered_map<LogLevel, spdlog::level::level_enum> m_LevelMap
      = {{LogLevel::None, spdlog::level::off},   {LogLevel::Fatal, spdlog::level::critical},
         {LogLevel::Error, spdlog::level::err},  {LogLevel::Warn, spdlog::level::warn},
         {LogLevel::Info, spdlog::level::info},  {LogLevel::Debug, spdlog::level::debug},
         {LogLevel::Trace, spdlog::level::trace}};

  bool Logger::Init(LogLevel logLevel, int logSinks) {
    if (s_Initialized) {
      return true;
    }

    std::vector<spdlog::sink_ptr> logSinksVec;

    // stdout color sink
    if (logSinks & LogSink::ColorStdOut) {
      logSinksVec.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
      if (!logSinksVec[0]) return false;
      logSinksVec[0]->set_pattern("[%T] [%^%l%$] %v");
    }

    // basic file sink
    if (logSinks & LogSink::BasicFile) {
      logSinksVec.emplace_back(
          std::make_shared<spdlog::sinks::basic_file_sink_mt>("rogue.log", true));
      if (!logSinksVec[1]) return false;
      logSinksVec[1]->set_pattern("[%T] [%l] %v");
    }

    // logger initialization
    s_Logger = std::make_shared<spdlog::logger>("Rogue", begin(logSinksVec), end(logSinksVec));
    if (!s_Logger) return false;
    spdlog::register_logger(s_Logger);
    s_Logger->set_level(m_LevelMap[logLevel]);
    s_Logger->flush_on(m_LevelMap[logLevel]);

    s_Initialized = true;

    return true;
  }

  void Logger::SetLogLevel(LogLevel logLevel) { s_Logger->set_level(m_LevelMap[logLevel]); }

}  // namespace Rogue
