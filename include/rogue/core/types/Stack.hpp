#ifndef STACK_H_
#define STACK_H_

#include <cstdint>

#include "rogue/core/utils/Assertion.hpp"

namespace Rogue {

  template <typename T> class Stack {
  public:
    Stack(uint32_t size) : m_Size(size) {
      m_Elements = new T[size];
      m_HeadIndex = 0;
    }

    ~Stack() { delete[] m_Elements; }

    uint32_t GetSize() const { return m_Size; }
    uint32_t GetCount() const { return m_HeadIndex; }
    bool IsEmpty() { return m_HeadIndex == 0; }
    bool IsFull() { return m_HeadIndex == m_Size; }

    void Push(const T& element) {
      if (IsFull()) ROGUE_ASSERT(false, "The Stack is full!");

      m_Elements[m_HeadIndex] = element;
      m_HeadIndex++;
    }

    const T& Pop() {
      if (IsEmpty()) ROGUE_ASSERT(false, "The Stack is empty!");

      m_HeadIndex--;
      return m_Elements[m_HeadIndex];
    }

    const T& Peek() {
      if (IsEmpty()) ROGUE_ASSERT(false, "The Stack is empty!");

      return m_Elements[m_HeadIndex - 1];
    }

  private:
    uint32_t m_Size;
    uint32_t m_HeadIndex;
    T* m_Elements;
  };

}  // namespace Rogue

#endif  // STACK_H_
