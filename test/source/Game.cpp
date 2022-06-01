#include <doctest/doctest.h>

#include <rogue/core/Game.hpp>

class TestGame : public Rogue::Game {
public:
  TestGame(int argc, char* argv[]) : Game(argc, argv) {
    (void)argc;
    (void)argv;
  }

  virtual bool ClientSetup() override { return true; }
  virtual void ClientUpdate() override {}
};

Rogue::Game* Rogue::CreateGame(int argc, char* argv[]) { return new TestGame(argc, argv); }

TEST_CASE("Game") {
  using namespace Rogue;

  Game* testGame = Rogue::CreateGame(0, nullptr);
  CHECK(testGame->EngineSetup() == true);
  CHECK(testGame->ClientSetup() == true);

  delete testGame;
}
