#ifndef DEBUGBREAK_H_
#define DEBUGBREAK_H_

//  --== DEBUG BREAKING ==--
#if (defined(__debugbreak) || (_MSC_VER))
#  define ROGUE_DEBUG_BREAK __debugbreak
#elif (defined(__breakpoint) || (__ARMCC_VERSION))
#  define ROGUE_DEBUGBREAK __breakpoint(42)
#else
#  include <signal.h>
#  if (defined(SIGTRAP))
#    define ROGUE_DEBUG_BREAK raise(SIGTRAP)
#  else
#    define ROGUE_DEBUG_BREAK raise(SIGABRT)
#  endif
#endif

#endif  // DEBUGBREAK_H_
