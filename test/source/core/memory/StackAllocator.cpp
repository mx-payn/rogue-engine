#include "rogue/core/memory/StackAllocator.hpp"

#include <doctest/doctest.h>

struct Test {
  int test = 1337;

  Test(int t) : test(t) {}
};

TEST_CASE("StackAllocator - simple allocation/deallocation") {
  Rogue::StackAllocator allocator;
  CHECK(allocator.GetObjectCount() == 0);
  Test* t1 = allocator.Allocate<Test>(42);
  CHECK(t1->test == 42);
  CHECK(allocator.GetObjectCount() == 1);
  Test* t2 = allocator.Allocate<Test>(42);
  CHECK(t2->test == 42);
  CHECK(allocator.GetObjectCount() == 2);
  Test* t3 = allocator.Allocate<Test>(42);
  CHECK(t3->test == 42);
  CHECK(allocator.GetObjectCount() == 3);

  allocator.PopAll();
  CHECK(allocator.GetObjectCount() == 0);
}

TEST_CASE("StackAllocator - Marker") {
  Rogue::StackAllocator allocator;
  Test* t1 = allocator.Allocate<Test>(42);
  (void)t1;
  CHECK(allocator.GetObjectCount() == 1);
  Test* t2 = allocator.Allocate<Test>(42);
  (void)t2;
  allocator.PushMarker();
  CHECK(allocator.GetObjectCount() == 2);
  Test* t3 = allocator.Allocate<Test>(42);
  (void)t3;
  CHECK(allocator.GetObjectCount() == 3);
  allocator.PopMarker();
  CHECK(allocator.GetObjectCount() == 2);
  allocator.PopAll();
}
