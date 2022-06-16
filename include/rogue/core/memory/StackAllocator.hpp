#ifndef STACKALLOCATOR_H_
#define STACKALLOCATOR_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "rogue/core/math/Util.hpp"
#include "rogue/core/types/Stack.hpp"
#include "rogue/core/utils/Assertion.hpp"

namespace Rogue {

  typedef int8_t byte;

  class StackAllocator {
  public:
    explicit StackAllocator(size_t size = 1024)
        : m_Size(size),
          m_ObjectContexts(size / sizeof(ObjectContext)),
          m_Markers(size / sizeof(byte*)) {
      m_Data = new byte[size];
      m_Head = m_Data;
    }

    StackAllocator(const StackAllocator& other) = delete;             // Copy Constructor
    StackAllocator(StackAllocator&& other) = delete;                  // Move Constructor
    StackAllocator& operator=(const StackAllocator& other) = delete;  // Copy Assignment
    StackAllocator& operator=(StackAllocator&& other) = delete;       // Move Assignment

    ~StackAllocator() {
      ROGUE_ASSERT(m_Data == m_Head, "Destructor called with object still on stack!");

      delete m_Data;
      m_Data = nullptr;
      m_Head = nullptr;
    }

    template <typename T, typename... Args> T* Allocate(Args&&... args) {
      byte* allocationLocation = (byte*)NextMultiple(alignof(T), (size_t)m_Head);
      byte* root = m_Head;
      byte* newHeadPointer = allocationLocation + sizeof(T);

      if (newHeadPointer <= m_Data + m_Size) {
        T* returnPointer = reinterpret_cast<T*>(allocationLocation);
        m_Head = newHeadPointer;
        T* object = new (returnPointer) T(args...);

        AddContextToStack(object, root);

        return returnPointer;
      } else {
        return nullptr;
      }
    }

    void DeallocateAll() {
      m_Head = m_Data;
      while (!m_ObjectContexts.IsEmpty()) {
        auto context = m_ObjectContexts.Pop();
        context();
      }

      m_Markers.Clear();
    }

    void PushMarker() { m_Markers.Push(m_Head); }

    void PopMarker() {
      ROGUE_ASSERT(!m_Markers.IsEmpty(), "No markers to pop!");

      byte* marker = m_Markers.Pop();
      ObjectContext current = m_ObjectContexts.Peek();
      while (marker != m_Head) {
        m_Head = current.Root;
        current();
        m_ObjectContexts.Pop();
        current = m_ObjectContexts.Peek();
      }
    }

    uint32_t GetObjectCount() const { return m_ObjectContexts.GetCount(); }

  private:
    struct ObjectContext {
    public:
      const void* Data;
      byte* Root;
      void (*destructor)(const void*);

      ObjectContext() : Data(nullptr), Root(nullptr) {}

      template <typename T> explicit ObjectContext(const T& data, byte* root)
          : Data(std::addressof(data)), Root(root) {
        destructor = [](const void* lambdaData) {
          auto originalType = static_cast<const T*>(lambdaData);
          originalType->~T();
        };
      }

      void operator()() { destructor(Data); }
    };

  private:
    byte* m_Data = nullptr;
    byte* m_Head = nullptr;
    size_t m_Size = 0;

    Stack<ObjectContext> m_ObjectContexts;
    Stack<byte*> m_Markers;

    template <typename T> inline void AddContextToStack(T* object, byte* root) {
      m_ObjectContexts.Push(ObjectContext(*object, root));
    }
  };

}  // namespace Rogue

#endif  // STACKALLOCATOR_H_
