#include "rogue/core/Game.hpp"

namespace Rogue {

  Game::Game(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
  }
  bool Game::EngineSetup() { return true; }
  void Game::Run() { m_Running = true; }
  void Game::OnEvent() {}

}  // namespace Rogue
