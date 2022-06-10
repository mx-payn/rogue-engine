#include "rogue/core/types/Stack.hpp"

#include <doctest/doctest.h>

TEST_CASE("Stack - Push Peek Pop") {
  Rogue::Stack<int> stack(10);
  CHECK(stack.GetCount() == 0);
  CHECK(stack.GetSize() == 10);
  CHECK(stack.IsEmpty() == true);
  CHECK(stack.IsFull() == false);

  for (int i = 0; i < 9; i++) {
    stack.Push(i);
    CHECK(stack.GetCount() == i + 1);
    CHECK(stack.IsEmpty() == false);
    CHECK(stack.IsFull() == false);
  }

  stack.Push(9);
  CHECK(stack.GetCount() == 10);
  CHECK(stack.IsEmpty() == false);
  CHECK(stack.IsFull() == true);

  for (int i = 10; i > 1; i--) {
    CHECK(stack.Peek() == i - 1);
    int value = stack.Pop();
    CHECK(value == i - 1);
    CHECK(stack.GetCount() == i - 1);
    CHECK(stack.IsEmpty() == false);
    CHECK(stack.IsFull() == false);
  }

  CHECK(stack.Peek() == 0);
  int value = stack.Pop();
  CHECK(value == 0);
  CHECK(stack.GetCount() == 0);
  CHECK(stack.IsEmpty() == true);
  CHECK(stack.IsFull() == false);
}
