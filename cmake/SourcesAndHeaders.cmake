set(
  rogue-headers include/rogue/core/Game.hpp include/rogue/core/utils/Logger.hpp
  include/rogue/core/utils/Assertion.hpp include/rogue/core/utils/DebugBreak.hpp
  include/rogue/core/memory/SmartPointer.hpp
  include/rogue/core/memory/PoolAllocator.hpp
  include/rogue/core/memory/StackAllocator.hpp
  include/rogue/core/types/Stack.hpp
)

set(rogue-sources source/core/Game.cpp source/core/utils/Logger.cpp include/rogue/EntryPoint.cpp)
