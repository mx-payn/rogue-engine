#ifndef ASSERTION_H_
#define ASSERTION_H_

#include <string>

#include "rogue/core/utils/DebugBreak.hpp"
#include "rogue/core/utils/Logger.hpp"

namespace Rogue {

  /** \brief A static assertion class.
   *
   *  Offers static function to assert expressions and log a description on failure.
   */
  class Assertion {
  public:
    /** \brief Evaluates expression and prints a debug message on fail.
     *
     * Assertion function to evaluate an expression and return the bool result.
     * Prints a LogLevel::Fatal message on expression failure with debug description.
     *
     * \param[in] exp The expression to evaluate.
     * \param[in] desc A description format string printed on failure.
     * \param[in] line Line number of the function call.
     * \param[in] file The filename of the function call.
     * \param[in] args Arguments to the format string.
     * \returns False on expression failure, else true.
     *
     */
    template <typename... Args> static bool AssertFn(bool exp, int line, const std::string& file,
                                                     const std::string& desc = std::string(),
                                                     Args... args) {
      if (exp) return true;

      // --> assertion failed
      if (!desc.empty()) {
        ROGUE_LOG_FATAL("ASSERTION FAILED -- {}:{}", file.c_str(), line);
        ROGUE_LOG_FATAL(desc, args...);
      }

      return false;
    }

  private:
    Assertion() = default;
  };
}  // namespace Rogue

//////////////
//
// ==> MACROS
//
////////
#define ROGUE_ASSERT(exp, ...)                                                                   \
  if (!::Rogue::Assertion::AssertFn(static_cast<int>(exp), __LINE__, __FILE__, ##__VA_ARGS__)) { \
    ROGUE_DEBUG_BREAK;                                                                           \
  }
#define ROGUE_ASSERT_EQ(exp1, exp2, ...)                                                    \
  if (!::Rogue::Assertion::AssertFn(static_cast<int>((exp1) == (exp2)), __LINE__, __FILE__, \
                                    ##__VA_ARGS__)) {                                       \
    ROGUE_DEBUG_BREAK;                                                                      \
  }
#define ROGUE_ASSERT_NOT_EQ(exp1, exp2, ...)                                                \
  if (!::Rogue::Assertion::AssertFn(static_cast<int>((exp1) != (exp2)), __LINE__, __FILE__, \
                                    ##__VA_ARGS__)) {                                       \
    ROGUE_DEBUG_BREAK;                                                                      \
  }
#define ROGUE_ASSERT_NOT_NULL(value, ...)                                                   \
  if (!::Rogue::Assertion::AssertFn(static_cast<int>(value != nullptr), __LINE__, __FILE__, \
                                    ##__VA_ARGS__)) {                                       \
    ROGUE_DEBUG_BREAK;                                                                      \
  }

#endif  // ASSERTION_H_
