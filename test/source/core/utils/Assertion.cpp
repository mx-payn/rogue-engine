#include "rogue/core/utils/Assertion.hpp"

#include <doctest/doctest.h>

TEST_CASE("Assertion") {
  CHECK(Rogue::Assertion::AssertFn(true, 0, ""));
  CHECK(Rogue::Assertion::AssertFn(true, 0, "Test assertion with {} Text.", "format"));
  CHECK(!Rogue::Assertion::AssertFn(false, 0, ""));
}
