#ifndef SMARTPOINTER_H_
#define SMARTPOINTER_H_

#include <memory>

namespace Rogue {

  // --== unique pointer ==--
  template <typename T> using scope_ptr = std::unique_ptr<T>;
  template <typename T, typename... Args> constexpr scope_ptr<T> make_scope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  // --== shared pointer ==--
  template <typename T> using ref_ptr = std::shared_ptr<T>;
  template <typename T, typename... Args> constexpr ref_ptr<T> make_ref(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

}  // namespace Rogue

#endif  // SMARTPOINTER_H_
