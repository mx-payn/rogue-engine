#ifndef ENTRYPOINT_H_
#define ENTRYPOINT_H_

#include "rogue/core/Game.hpp"

int main(int argc, char* argv[]) {
  auto game = Rogue::CreateGame(argc, argv);

  game->EngineSetup();
  game->ClientSetup();
  game->Run();

  delete game;

  return 0;
}

#endif  // ENTRYPOINT_H_
