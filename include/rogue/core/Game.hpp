#ifndef GAME_H_
#define GAME_H_

namespace Rogue {

  /** \brief The top level engine class defining the engines runtime behaviour.
   *
   * TODO: describe this class
   */
  class Game {
  public:
    /** \brief The Game objects top level constructor.
     *
     *  Gets called very first on Game creation. It only initializes the Logger,
     *  to give the Client constructor the opportunity of processing the commandline args
     *  before subsystem initialization. Actual engine initialization is done after
     *  Client construction by EngineSetup().
     */
    Game(int argc, char* argv[]);

    /** A default Destructor */
    virtual ~Game() = default;

    /** \brief Client initialization.
     *
     *  Should be used for client application initializations. Gets
     *  called AFTER EngineSetup() whereas the Client constructor gets
     *  called BEFORE EngineSetup(). So all engine subsystem accessing
     *  calls have to be done here, as they have not been initialized yet on
     *  Client construction.
     *
     *  \returns True on success, else False.
     */
    virtual bool ClientSetup() = 0;

    /** \brief Update function implemented by Client.
     *
     *  Gets called every frame after subsystem updates. Gives
     *  the client application the opportunity to change layers
     *  or do other stuff.
     */
    virtual void ClientUpdate() = 0;

    /** \brief Subsystem initialization and asset loading.
     *
     *  Initializes all engine subsystems and loads all engine iternal assets.
     *  Order of initialization:
     *
     *  - Window
     *  - UpdateModule
     *  - AssetManager
     *  - RenderModule
     *  - Asset loading
     *
     *  \returns True on success, else False.
     */
    bool EngineSetup();

    /** \brief The game loop.
     *
     *  Defines the game loop. The order of processing is:
     *
     *  - delta time calculation
     *  - update
     *    - subsystems update
     *    - client update
     *    - layer update
     *  - rendering
     *    - layer rendering
     *    - imgui rendering
     *    - buffer swap
     *
     *  Exits game loop on m_Running toggle.
     */
    void Run();

    /** \brief The engine-internal Event callback function.
     *
     *  Gets passed as function pointer to classes as event callback function.
     *  Passes all received Events to relevant objects. OnEvent() is engine
     *  internal, but also passes Events to client defined Layers, as they get
     *  pushed on the LayerStack.
     */
    void OnEvent();

  private:
    bool m_Running = true;  //<! Game loop toggle.
  };

  /** \brief Definition to be implemented by the Client application IF
   *         predefined main() at EntryPoint.hpp is used.
   *
   *  Rogue offers a predefined main() function inside EntryPoint.hpp, which
   *  instantiates a Game by this definition and calls the Game setup and run
   *  functions. If EntryPoint.hpp is used, the Client application has to implement
   *  this function as fits.
   *
   *  Example:
   *
   *  \code{.cpp}
   *  #include <rogue/EntryPoint.hpp>
   *
   *  class ExampleGame : public Rogue::Game {
   *  public:
   *    ExampleGame(int argc, char* argv[]);
   *
   *      --== DEFINE YOUR GAME ==--
   *                  ...
   *  };
   *
   *  Rogue::Game* Rogue::CreateGame(int argc, char* argv[]) {
   *    return new ExampleGame(argc, argv);
   *  }
   *  \endcode
   *
   *  Note: It is not obligated to use EntryPoint.hpp, you can define your own main()
   *  function and call necessary Game functions yourself. See EntryPoint.hpp for
   *  reference.
   *
   *  \param[in] argc Forwarded program argument count.
   *  \param[in] argv Forwarded program arguments.
   *  \returns A pointer to the client defined game.
   *
   *  \see EntryPoint.hpp
   *
   */
  Game* CreateGame(int argc, char* argv[]);

}  // namespace Rogue

#endif  // GAME_H_
