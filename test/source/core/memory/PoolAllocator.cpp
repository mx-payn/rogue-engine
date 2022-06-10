#include "rogue/core/memory/PoolAllocator.hpp"

#include <doctest/doctest.h>

#include <cstdint>
#include <string>

#include "rogue/core/utils/Logger.hpp"

struct TestStruct {
  int Num = 0;
  std::string Text = "";

  TestStruct(int num, const std::string& text) : Num(num), Text(text) {}
};

TEST_CASE("Simple allocation/deallocation") {
  Rogue::PoolAllocator<TestStruct> allocator(32);
  CHECK(allocator.GetSize() == 32);
  CHECK(allocator.GetObjectCount() == 0);
  CHECK(allocator.GetFreeChunkCount() == 32);
  TestStruct* t1 = allocator.allocate(1337, "L33t");
  CHECK(allocator.GetObjectCount() == 1);
  CHECK(allocator.GetFreeChunkCount() == 31);
  CHECK(t1 != nullptr);
  CHECK(t1->Num == 1337);
  CHECK(t1->Text == "L33t");

  allocator.deallocate(t1);
  CHECK(allocator.GetObjectCount() == 0);
  CHECK(allocator.GetFreeChunkCount() == 32);
}

TEST_CASE("overallocation") {
  Rogue::PoolAllocator<TestStruct> allocator(10);
  CHECK(allocator.GetSize() == 10);
  CHECK(allocator.GetObjectCount() == 0);
  CHECK(allocator.GetFreeChunkCount() == 10);

  TestStruct* tArray[11];
  for (uint32_t i = 0; i < 11; i++) {
    tArray[i] = nullptr;
    tArray[i] = allocator.allocate(i, "Testing");

    if (i != 10) {
      CHECK(allocator.GetObjectCount() == i + 1);
      CHECK(allocator.GetFreeChunkCount() == 9 - i);
    } else {
      CHECK(allocator.GetObjectCount() == 10);
      CHECK(allocator.GetFreeChunkCount() == 0);
    }
  }

  for (uint32_t i = 0; i < 11; i++) {
    if (i == 10) {
      CHECK(tArray[i] == nullptr);
      continue;
    } else {
      CHECK(tArray[i] != nullptr);
    }
  }

  for (uint32_t i = 9; i <= 0; i--) {
    allocator.deallocate(tArray[i]);
  }
}

TEST_CASE("defragmented allocation") {
  Rogue::PoolAllocator<TestStruct> allocator(10);

  // allocate all testobjects
  TestStruct* tArray[10];
  for (uint32_t i = 0; i < 10; i++) {
    tArray[i] = nullptr;
    tArray[i] = allocator.allocate(i, "Testing");
  }

  // free one middle object
  allocator.deallocate(tArray[5]);

  // allocate new object, no allocator space should be available
  TestStruct* t5 = allocator.allocate(5, "NewAlloc1");
  CHECK(t5 != nullptr);
  // new pointer should have same address as deallocated pointer
  CHECK(t5 == tArray[5]);
  // allocate another object, allocator should not have free space
  // and return nullptr
  TestStruct* t6 = allocator.allocate(5, "NewAlloc2");
  CHECK(t6 == nullptr);
}
