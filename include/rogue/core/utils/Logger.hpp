#ifndef LOGGER_H_
#define LOGGER_H_

#include <spdlog/spdlog.h>

#include <memory>

namespace Rogue {

  enum class LogLevel { None, Fatal, Error, Warn, Info, Debug, Trace };

  class Logger {
  public:
    static void Init();
    static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
    static void SetLogLevel(LogLevel logLevel);

  private:
    Logger() = default;
    static std::shared_ptr<spdlog::logger> s_Logger;
  };

#define ROGUE_LOG_TRACE(...) ::Rogue::Logger::GetLogger()->trace(__VA_ARGS__)
#define ROGUE_LOG_DEBUG(...) ::Rogue::Logger::GetLogger()->debug(__VA_ARGS__)
#define ROGUE_LOG_INFO(...) ::Rogue::Logger::GetLogger()->info(__VA_ARGS__)
#define ROGUE_LOG_WARN(...) ::Rogue::Logger::GetLogger()->warn(__VA_ARGS__)
#define ROGUE_LOG_ERROR(...) ::Rogue::Logger::GetLogger()->error(__VA_ARGS__)
#define ROGUE_LOG_FATAL(...) ::Rogue::Logger::GetLogger()->critical(__VA_ARGS__)

}  // namespace Rogue

#endif  // LOGGER_H_
