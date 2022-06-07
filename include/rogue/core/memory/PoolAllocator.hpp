#ifndef POOLALLOCATOR_H_
#define POOLALLOCATOR_H_

#include <cstddef>
#include <memory>
#include <utility>

#define ROGUE_POOL_ALLOCATOR_DEFAULT_SIZE 1024

namespace rogue {

  template <typename T> union PoolChunk {
    T Value;
    PoolChunk<T>* NextPoolChunk;

    PoolChunk() = default;
    ~PoolChunk() = default;
  };

  template <typename T> class PoolAllocator {
  public:
    explicit PoolAllocator(size_t size = ROGUE_POOL_ALLOCATOR_DEFAULT_SIZE) : m_Size(size) {
      m_Data = new PoolChunk<T>[size];
      m_Head = m_Data;

      for (size_t i = 0; i < m_Size - 1; i++)
        m_Data[i].NextPoolChunk = std::addressof(m_Data[i + 1]);

      m_Data[m_Size - 1].NextPoolChunk = nullptr;
    }

    PoolAllocator(const PoolAllocator& other) = delete;              // Copy Constructor
    PoolAllocator(const PoolAllocator&& other) = delete;             // Move Constructor
    PoolAllocator& operator=(const PoolAllocator& other) = delete;   // Copy Assignment
    PoolAllocator& operator=(const PoolAllocator&& other) = delete;  // Move Assignment

    ~PoolAllocator() {
      delete[] m_Data;
      m_Data = nullptr;
      m_Head = nullptr;
    }

    template <typename... Args> T* allocate(Args&&... args) {
      if (m_Head == nullptr) return nullptr;  // no more space to allocate

      PoolChunk<T>* poolChunk = m_Head;
      m_Head = m_Head->NextPoolChunk;
      T* returnValue = new (std::addressof(poolChunk->Value)) T(std::forward<Args>(args)...);

      return returnValue;
    }

    void deallocate(T* data) {
      // check data actually was allocated here
      ROGUE_ASSERT(data >= m_Data && data <= std::addressof(m_Data[m_Size - 1]));

      data->~T();
      PoolChunk<T>* poolChunk = reinterpret_cast<PoolChunk<T>*>(data);
      poolChunk->NextPoolChunk = m_Head;
      m_Head = poolChunk;
    }

  private:
    size_t m_Size = 0;
    PoolChunk<T>* m_Data = nullptr;
    PoolChunk<T>* m_Head = nullptr;
  };

}  // namespace rogue

#endif  // POOLALLOCATOR_H_
