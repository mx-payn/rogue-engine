#ifndef LOGGER_H_
#define LOGGER_H_

#include <spdlog/spdlog.h>

#include <memory>

namespace Rogue {

  /**
   * Defines the possible threshold levels for logging output. The Logger only
   * processes logging events upwards and including the chosen level. Order of levels:
   *
   * _
   * ^  None
   * |  Fatal
   * ^  Error
   * |  Warn
   * ^  Info
   * |  Debug
   * ^  Trace
   */
  enum class LogLevel { None, Fatal, Error, Warn, Info, Debug, Trace };

  /**
   * The possible sinks to be used by the spdlog::logger. Pass bitmask to
   * Logger::Init() function.
   */
  enum LogSink { BasicStdOut = 1 << 0, ColorStdOut = 1 << 1, BasicFile = 1 << 2 };

  /** \brief Static spdlog wrapper
   *
   *  Logger wraps spdlog::logger. This class only contains static functions and
   *  members, Constructor is private by design. The logging functions are defined
   *  as macros below.
   */
  class Logger {
  public:
    /** \brief Initializing logger member and settings
     *
     *  Initializes the static member s_Logger and registers it as the
     *  default logger. Also adds chosen predefined sinks to the logger, passed
     *  via Bitmask of LogSink. See LogSink enum for possible sinks.
     *
     *  \param[in] logLevel The initial logLevel output threshold.
     *  \param[in] logSinks The bitmask for logSinks to be enabled.
     *  \returns True on success, else false.
     */
    static bool Init(LogLevel logLevel, int logSinks = 0);

    // -- Getter / Setter
    static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
    static void SetLogLevel(LogLevel logLevel);

  private:
    Logger() = default;
    static std::shared_ptr<spdlog::logger> s_Logger;  //<! static spdlog::logger object.
  };

  //  --== Macros ==--
#define ROGUE_LOG_TRACE(...) ::Rogue::Logger::GetLogger()->trace(__VA_ARGS__)
#define ROGUE_LOG_DEBUG(...) ::Rogue::Logger::GetLogger()->debug(__VA_ARGS__)
#define ROGUE_LOG_INFO(...) ::Rogue::Logger::GetLogger()->info(__VA_ARGS__)
#define ROGUE_LOG_WARN(...) ::Rogue::Logger::GetLogger()->warn(__VA_ARGS__)
#define ROGUE_LOG_ERROR(...) ::Rogue::Logger::GetLogger()->error(__VA_ARGS__)
#define ROGUE_LOG_FATAL(...) ::Rogue::Logger::GetLogger()->critical(__VA_ARGS__)

}  // namespace Rogue

#endif  // LOGGER_H_
