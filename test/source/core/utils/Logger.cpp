
#include "rogue/core/utils/Logger.hpp"

#include <doctest/doctest.h>

TEST_CASE("Logger") {
  CHECK(Rogue::Logger::Init(Rogue::LogLevel::Trace) == true);
  CHECK(Rogue::Logger::GetLogger().get() != nullptr);
  CHECK(Rogue::Logger::GetLogger()->level() == spdlog::level::level_enum::trace);

  Rogue::Logger::SetLogLevel(Rogue::LogLevel::None);
  CHECK(Rogue::Logger::GetLogger()->level() == spdlog::level::level_enum::off);
  Rogue::Logger::SetLogLevel(Rogue::LogLevel::Fatal);
  CHECK(Rogue::Logger::GetLogger()->level() == spdlog::level::level_enum::critical);
  Rogue::Logger::SetLogLevel(Rogue::LogLevel::Error);
  CHECK(Rogue::Logger::GetLogger()->level() == spdlog::level::level_enum::err);
  Rogue::Logger::SetLogLevel(Rogue::LogLevel::Warn);
  CHECK(Rogue::Logger::GetLogger()->level() == spdlog::level::level_enum::warn);
  Rogue::Logger::SetLogLevel(Rogue::LogLevel::Info);
  CHECK(Rogue::Logger::GetLogger()->level() == spdlog::level::level_enum::info);
  Rogue::Logger::SetLogLevel(Rogue::LogLevel::Debug);
  CHECK(Rogue::Logger::GetLogger()->level() == spdlog::level::level_enum::debug);
  Rogue::Logger::SetLogLevel(Rogue::LogLevel::Trace);
  CHECK(Rogue::Logger::GetLogger()->level() == spdlog::level::level_enum::trace);
}
