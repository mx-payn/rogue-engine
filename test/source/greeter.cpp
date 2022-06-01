#include <doctest/doctest.h>
#include <greeter/greeter.h>
#include <rogue/version.h>

#include <string>

TEST_CASE("Greeter") {
  using namespace greeter;

  Greeter greeter("Tests");

  CHECK(greeter.greet(LanguageCode::EN) == "Hello, Tests!");
  CHECK(greeter.greet(LanguageCode::DE) == "Hallo Tests!");
  CHECK(greeter.greet(LanguageCode::ES) == "¡Hola Tests!");
  CHECK(greeter.greet(LanguageCode::FR) == "Bonjour Tests!");
}

TEST_CASE("Rogue version") {
  static_assert(std::string_view(ROGUE_VERSION) == std::string_view("0.0.1"));
  CHECK(std::string(ROGUE_VERSION) == std::string("0.0.1"));
}
