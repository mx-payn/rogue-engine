#ifndef POOLALLOCATOR_H_
#define POOLALLOCATOR_H_

#include <cstddef>

#include "rogue/core/types/Stack.hpp"
#include "rogue/core/utils/Assertion.hpp"

#define ROGUE_POOL_ALLOCATOR_DEFAULT_AMOUNT 1024

namespace Rogue {

  /** \brief Heap based allocator to construct multiple objects of a single type.
   *
   *  Provides functions to construct and destruct multiple objects up to given size on the
   *  internally reserved memory space.
   *
   *  Free chunks to hold objects are tracked by a stack container. Last deallocated
   *  chunk will be used for next allocation. At construction, free chunks are pushed in
   *  reversed order (lowest address on top).
   */
  template <typename T> class PoolAllocator {
  public:
    /** Reserves memory for given type T and amount of objects. Pushes the free
     *  chunks on the stack in reversed order (lowest address on top).
     *
     *  \param[in] amount Maximum amount of objects to be allocated.
     */
    explicit PoolAllocator(uint32_t amount = ROGUE_POOL_ALLOCATOR_DEFAULT_AMOUNT)
        : m_Size(amount), m_FreeChunks(amount) {
      // allocate amount of objects / chunks
      m_First = (T*)malloc(sizeof(T) * amount);
      m_Last = m_First + (amount - 1);

      // push free chunks on the stack
      for (int i = m_Size - 1; i >= 0; i--) {
        m_FreeChunks.Push(m_First + i);
      }
    }

    // rule of five
    PoolAllocator(const PoolAllocator& other) = delete;             // Copy Constructor
    PoolAllocator(PoolAllocator&& other) = delete;                  // Move Constructor
    PoolAllocator& operator=(const PoolAllocator& other) = delete;  // Copy Assignment
    PoolAllocator& operator=(PoolAllocator&& other) = delete;       // Move Assignment
    ~PoolAllocator() {
      delete m_First;
      m_First = nullptr;
      m_Last = nullptr;
    }

    /** Constructs an object of type T at last pushed free-chunk address.
     *
     *  \param[in] args Arguments to construct the object.
     *  \returns Typed pointer to allocated object, or nullptr if no available space.
     */
    template <typename... Args> T* allocate(Args&&... args) {
      if (m_FreeChunks.IsEmpty()) return nullptr;  // no more space to allocate

      T* poolChunk = m_FreeChunks.Pop();

      // placment new of object
      T* returnValue = new (poolChunk) T(std::forward<Args>(args)...);

      return returnValue;
    }

    /** Calls the deconstuctor of the object at given address and pushes the freeed-chunk
     *  on the stack.
     *
     *  Checks whether the object actually lies inside the data space. Debug breaks
     *  if passed pointer is outside of the data range.
     *
     *  \param[in] data The address of the object to be deallocated.
     */
    void deallocate(T* data) {
      // check if pointer was allocated here
      bool lower = data >= m_First;
      bool upper = data <= m_Last;
      ROGUE_ASSERT(lower && upper, "Pointer was not allocated here!");

      // object dextructor call and resetting head
      data->~T();
      m_FreeChunks.Push(data);
    }

    //  -- Getter
    uint32_t GetObjectCount() const { return m_Size - m_FreeChunks.GetCount(); }
    size_t GetFreeChunkCount() const { return m_FreeChunks.GetCount(); }
    size_t GetSize() const { return m_Size; }

  private:
    uint32_t m_Size = 0;   //!< Amount of objects that can be allocated.
    T* m_First = nullptr;  //!< Beginning of address space.
    T* m_Last = nullptr;   //!< End of address space.

    Stack<T*> m_FreeChunks;  //!< Stack container holding addresses to all free chunks
  };

}  // namespace Rogue

#endif  // POOLALLOCATOR_H_
