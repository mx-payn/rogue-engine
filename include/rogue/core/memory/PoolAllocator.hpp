#ifndef POOLALLOCATOR_H_
#define POOLALLOCATOR_H_

#include <cstddef>

#include "rogue/core/types/Stack.hpp"
#include "rogue/core/utils/Assertion.hpp"

#define ROGUE_POOL_ALLOCATOR_DEFAULT_SIZE 1024

namespace Rogue {

  template <typename T> class PoolAllocator {
  public:
    explicit PoolAllocator(size_t size = ROGUE_POOL_ALLOCATOR_DEFAULT_SIZE)
        : m_Size(size), m_FreeChunks(size) {
      // allocate amount of objects / chunks
      m_First = (T*)malloc(sizeof(T) * size);
      m_Last = m_First + (size - 1);

      for (int i = m_Size - 1; i >= 0; i--) {
        m_FreeChunks.Push(m_First + i);
      }
    }

    PoolAllocator(const PoolAllocator& other) = delete;              // Copy Constructor
    PoolAllocator(const PoolAllocator&& other) = delete;             // Move Constructor
    PoolAllocator& operator=(const PoolAllocator& other) = delete;   // Copy Assignment
    PoolAllocator& operator=(const PoolAllocator&& other) = delete;  // Move Assignment

    ~PoolAllocator() {
      delete m_First;
      m_First = nullptr;
      m_Last = nullptr;
    }

    template <typename... Args> T* allocate(Args&&... args) {
      if (m_FreeChunks.IsEmpty()) return nullptr;  // no more space to allocate

      T* poolChunk = m_FreeChunks.Pop();

      // placment new of object
      T* returnValue = new (poolChunk) T(std::forward<Args>(args)...);

      return returnValue;
    }

    void deallocate(T* data) {
      // check if pointer was allocated here
      bool lower = data >= m_First;
      bool upper = data <= m_Last;
      ROGUE_ASSERT(lower && upper, "Pointer was not allocated here!");

      // object dextructor call and resetting head
      data->~T();
      m_FreeChunks.Push(data);
    }

    uint32_t GetObjectCount() const { return m_Size - m_FreeChunks.GetCount(); }
    size_t GetFreeChunkCount() const { return m_FreeChunks.GetCount(); }
    size_t GetSize() const { return m_Size; }

  private:
    size_t m_Size = 0;
    T* m_First = nullptr;
    T* m_Last = nullptr;

    Stack<T*> m_FreeChunks;
  };

}  // namespace Rogue

#endif  // POOLALLOCATOR_H_
