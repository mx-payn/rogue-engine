#include "rogue/core/memory/SmartPointer.hpp"

#include <doctest/doctest.h>

struct Test_Struct {
  int member = 1337;
};

TEST_CASE("scope_ptr") {
  Rogue::scope_ptr<Test_Struct> ptr1 = Rogue::make_scope<Test_Struct>();
  CHECK(ptr1.get() != nullptr);
  CHECK(ptr1->member == 1337);
}

TEST_CASE("ref_ptr") {
  Rogue::ref_ptr<Test_Struct> ptr1 = Rogue::make_ref<Test_Struct>();
  CHECK(ptr1.get() != nullptr);
  CHECK(ptr1->member == 1337);
  CHECK(ptr1.use_count() == 1);

  {
    Rogue::ref_ptr<Test_Struct> ptr2 = ptr1;
    CHECK(ptr2.get() != nullptr);
    CHECK(ptr2->member == 1337);
    CHECK(ptr1.use_count() == 2);
    CHECK(ptr2.use_count() == 2);
  }

  CHECK(ptr1.use_count() == 1);
}
