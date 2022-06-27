#ifndef STACKALLOCATOR_H_
#define STACKALLOCATOR_H_

#include <stddef.h>
#include <stdint.h>

#include <new>
#include <utility>

#include "rogue/core/math/Util.hpp"
#include "rogue/core/types/Stack.hpp"
#include "rogue/core/utils/Assertion.hpp"

namespace Rogue {

  typedef int8_t byte;

  /** \brief Stack based object allocator with fixed size.
   *
   *  Provides a stack based memory management system for the specified
   *  amount of bytes. Types are allocated by template and deallocated by either
   *  popping to a set marker or by clearing the entire space. Deconstructor
   *  of the object is called upon deallocation.
   */
  class StackAllocator {
  public:
    explicit StackAllocator(uint32_t size = 1024)
        : m_Size(size),
          m_ObjectContexts(size / sizeof(ObjectContext)),
          m_Markers(size / sizeof(byte*)) {
      m_Data = new byte[size];
      m_Head = m_Data;
    }

    // rule of five
    StackAllocator(const StackAllocator& other) = delete;             // Copy Constructor
    StackAllocator(StackAllocator&& other) = delete;                  // Move Constructor
    StackAllocator& operator=(const StackAllocator& other) = delete;  // Copy Assignment
    StackAllocator& operator=(StackAllocator&& other) = delete;       // Move Assignment
    ~StackAllocator() {
      ROGUE_ASSERT(m_Data == m_Head, "Destructor called with [{}] objects still on stack!",
                   m_ObjectContexts.GetCount())

      delete m_Data;
      m_Data = nullptr;
      m_Head = nullptr;
    }

    /** \brief Allocates object constructed with given args on the internal stack.
     *
     *  Allocation template function to allocate an object of template type T
     *  on the class internal stack based memory. Object location is aligned by
     *  the alignment information of provided type T.
     *
     *  Object is constructed by placement new at the aligned location with given args.
     *  If not enough space is available to construct the object, a nullptr is returned,
     *  else the typed pointer to constructed object is returned.
     *
     *  \param[in] args Arguments to construct the object of type T.
     *  \returns Typed pointer to the constructed object, or nullptr if not enough space available.
     */
    template <typename T, typename... Args> T* Allocate(Args&&... args) {
      // calculate address of object, considering the alignment of its type
      byte* allocationLocation = (byte*)NextMultiple(alignof(T), (size_t)m_Head);
      // move head to next free address
      byte* newHeadPointer = allocationLocation + sizeof(T);

      if (newHeadPointer <= m_Data + m_Size) {  // space available
        T* returnPointer = reinterpret_cast<T*>(allocationLocation);
        // the root address of the object, not considering the alignment
        // needed to free memory and move head pointer to the correct address
        // without any alignment gaps
        byte* root = m_Head;
        m_Head = newHeadPointer;

        // placement new of the object
        T* object = new (returnPointer) T(args...);

        // save information neccessary for destruction
        AddContextToStack(object, root);

        return returnPointer;
      } else {  // no space available
        return nullptr;
      }
    }

    /** Deallocates all objects on the stack and calls each deconstructor.
     *  All pushed markers are deleted and the stack head is set to the very beginning
     *  of available memory address space.
     */
    void PopAll() {
      // destruct all objects
      while (!m_ObjectContexts.IsEmpty()) {
        auto context = m_ObjectContexts.Pop();
        context.Destruct();
      }

      // reset head and markers
      m_Head = m_Data;
      m_Markers.Clear();
    }

    /** Pushes a marker at current head on the marker-stack. Objects with root
     *  address greater or equal the markers address may then be deallocated
     *  by PopMarker().
     */
    void PushMarker() { m_Markers.Push(m_Head); }

    /** Deallocates and destructs all objects with root address greater or equal to the marker
     *  on top of the marker-stack. Head is set to markers address.
     */
    void PopMarker() {
      ROGUE_ASSERT(!m_Markers.IsEmpty(), "No markers to pop!")

      byte* marker = m_Markers.Pop();
      ObjectContext current;
      while (marker != m_Head) {
        // pop last object
        current = m_ObjectContexts.Pop();
        // set head to objects root, which leaves no alignment gap
        m_Head = current.Root;
        // destruct current with overloaded () operator
        current.Destruct();
      }
    }

    /** \returns The amount of currently allocated objects on the stack */
    uint32_t GetObjectCount() const { return m_ObjectContexts.GetCount(); }

  private:  // -- classes and structs
    /** Data container holding information about address and destruction of
     *  an allocated object.
     */
    struct ObjectContext {
      const void* Data;  //!< The aligned address of the object.
      byte* Root;        //!< The original address (unaligned) of the object.

      // default Constructor only necessary to construct the stack
      ObjectContext() : Data(nullptr), Root(nullptr) {}

      /** Constructs the context of given object and sets the destruction function.
       *  The destructor function simply casts the object to its original type and
       *  calls the destructor on it.
       */
      template <typename T> explicit ObjectContext(const T& data, byte* root)
          : Data(std::addressof(data)), Root(root) {
        Destructor = [](const void* lambdaData) {
          auto originalType = static_cast<const T*>(lambdaData);
          originalType->~T();
        };
      }

      /** Function call to destruct this contexts object. */
      void Destruct() { Destructor(Data); }

    private:
      void (*Destructor)(
          const void*);  //!< The function ptr to the objects destruction information.
    };

  private:  // -- functions
    /** Readability function to push the context of an allocated object on the stack. */
    template <typename T> inline void AddContextToStack(T* object, byte* root) {
      m_ObjectContexts.Push(ObjectContext(*object, root));
    }

  private:                   // -- variables
    byte* m_Data = nullptr;  //!< The available space on the stack.
    byte* m_Head = nullptr;  //!< The address on the very top of the stack.
    uint32_t m_Size = 0;     //!< The size of the stack in bytes.

    Stack<ObjectContext> m_ObjectContexts;  //!< Container holding allocated object information
                                            //!< in the allocated order (last allocated on top).
    Stack<byte*> m_Markers;  //!< Container holding all deallocation markers in the pushed
                             //!< order (last pushed marker on top).
  };

}  // namespace Rogue

#endif  // STACKALLOCATOR_H_
