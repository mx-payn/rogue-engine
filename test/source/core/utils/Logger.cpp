
#include "rogue/core/utils/Logger.hpp"

#include <doctest/doctest.h>

TEST_CASE("Logger") {
  CHECK(Rogue::Logger::Init(Rogue::LogLevel::Trace) == true);
  CHECK(Rogue::Logger::GetLogger()->level() == spdlog::level::level_enum::trace);

  Rogue::Logger::SetLogLevel(Rogue::LogLevel::Info);
  CHECK(Rogue::Logger::GetLogger()->level() == spdlog::level::level_enum::info);
}
