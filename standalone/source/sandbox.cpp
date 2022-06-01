#include "rogue/EntryPoint.cpp"

class Sandbox : public Rogue::Game {
public:
  Sandbox(int argc, char* argv[]) : Game(argc, argv) {
    (void)argc;
    (void)argv;
  }

  ~Sandbox() = default;

  virtual bool ClientSetup() override { return true; }
  virtual void ClientUpdate() override {}
};

Rogue::Game* Rogue::CreateGame(int argc, char* argv[]) { return new Sandbox(argc, argv); }
