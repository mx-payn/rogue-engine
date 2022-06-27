#include "rogue/core/utils/Assertion.hpp"

#include <doctest/doctest.h>

TEST_CASE("Assertion") {
  Rogue::Logger::Init(Rogue::LogLevel::Trace);
  CHECK(Rogue::Assertion::AssertFn(true, 0, "Assertion.cpp"));
  CHECK(Rogue::Assertion::AssertFn(true, 0, "Assertion.cpp", "Test assertion with {} Text.",
                                   "format"));
  CHECK(!Rogue::Assertion::AssertFn(false, 0, "Assertion.cpp"));
  CHECK(!Rogue::Assertion::AssertFn(false, 0, "Assertion.cpp", "Test assertion with {} Text.",
                                    "format"));
}
