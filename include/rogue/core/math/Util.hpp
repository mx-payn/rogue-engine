#ifndef UTIL_H_
#define UTIL_H_

namespace Rogue {

  template <typename T> T NextMultiple(const T& multipleOf, const T& value) {
    T multiple = value + multipleOf - 1;
    multiple -= (multiple % multipleOf);
    return multiple;
  }

}  // namespace Rogue

#endif  // UTIL_H_
