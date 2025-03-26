/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    Nathan Bird, Brock Hoskins
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>
#include <memory>   // for std::allocator

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *****************************************************/
   template <typename T, typename A = std::allocator<T>>
   class deque
   {
      friend class ::TestDeque; // give unit tests access to the privates
   public:

      // 
      // Construct
      //
      deque(const A& a = A())
      {
         data = nullptr;
      }
      deque(deque& rhs);
      ~deque()
      {}

      //
      // Assign
      //
      deque& operator = (deque& rhs);

      // 
      // Iterator
      //
      class iterator;
      iterator begin()
      {
         return iterator(0, *this);
      }
      iterator end()
      {
         return iterator(numElements, *this);
      }

      // 
      // Access
      //
      T& front()
      {
         return *(new T);
      }
      const T& front() const
      {
         return *(new T);
      }
      T& back()
      {
         return *(new T);
      }
      const T& back() const
      {
         return *(new T);
      }
      T& operator[](int id)
      {
         return *(new T);
      }
      const T& operator[](int id) const
      {
         return *(new T);
      }

      //
      // Insert
      //
      void push_back(const T& t);
      void push_back(T&& t);
      void push_front(const T& t);
      void push_front(T&& t);

      //
      // Remove
      //
      void pop_front();
      void pop_back();
      void clear();

      //
      // Status
      //
      size_t size()  const { return 99; }
      bool   empty() const { return false; }

   private:
      // array index from deque index
      int iaFromID(int id) const
      {
         assert(0 <= id && id < numElements);
         assert(0 <= iaFront && iaFront < numCells * numBlocks);
         int ia = (id + iaFront) % (numCells * numBlocks);
         assert(0 <= ia && ia < numCells * numBlocks);
         return ia;
      }

      // block index from deque index
      int ibFromID(int id) const
      {
         int ib = iaFromID(id) / numCells;
         assert(0 <= ib && ib < numBlocks);
         return ib;
      }

      // cell index from deque index
      int icFromID(int id) const
      {
         int ic = iaFromID(id) % numCells;
         assert(0 <= ic && ic < numCells);
         return ic;
      }

      // reallocate
      void reallocate(int numBlocksNew);

      A      alloc;              // use alloacator for memory allocation
      size_t numCells;           // number of cells in a block
      size_t numBlocks;          // number of blocks in the data array
      size_t numElements;        // number of elements in the deque
      int    iaFront;            // array-centered index of the front of the deque
      T** data;               // array of arrays
   };

   /**************************************************
    * DEQUE ITERATOR
    * An iterator through deque.  You only need to
    * support the following:
    *   1. Constructors (default and copy)
    *   2. Not equals operator
    *   3. Increment (prefix and postfix)
    *   4. Dereference
    * This particular iterator is a bi-directional meaning
    * that ++ and -- both work.  Not all iterators are that way.
    *************************************************/
   template <typename T, typename A>
   class deque <T, A> ::iterator
   {
      friend class ::TestDeque; // give unit tests access to the privates
   public:
      // 
      // Construct
      //
      iterator()
      {}
      iterator(int id, deque* d)
      {
         this.id = id;
         this.d = d;
      }
      iterator(const iterator& rhs) : id(rhs.id), d(rhs.d)
      {}

      //
      // Assign
      //
      iterator& operator = (const iterator& rhs)
      {
         id = rhs.id;
         d = rhs.d;
         return *this;
      }

      // 
      // Compare
      //
      bool operator != (const iterator& rhs) const { return id != rhs.id && d != rhs.d; }
      bool operator == (const iterator& rhs) const { return id == rhs.id && d == rhs.d; }

      // 
      // Access
      //
      T& operator * ()
      {
         // return d[id];
      }

      // 
      // Arithmetic
      //
      int operator - (iterator it) const
      {
         return id - it.id;
      }
      iterator& operator += (int offset)
      {
         if (offset > 0)
         {
            while (offset--)
               ++(*this);
         }
         else if (offset < 0)
         {
            while (offset++)
               --(*this);
         }
         return *this;
      }
      iterator& operator ++ ()
      {
         ++id;
         return *this;
      }
      iterator operator ++ (int postfix)
      {
         iterator temp(*this);
         ++id;
         return temp;
      }
      iterator& operator -- ()
      {
         --id;
         return *this;
      }
      iterator operator -- (int postfix)
      {
         iterator temp(*this);
         --id;
         return temp;
      }

   private:
      int id;
      deque* d;
   };

   /*****************************************
    * DEQUE :: COPY CONSTRUCTOR
    * Allocate the space for the elements and
    * call the copy constructor on each element
    ****************************************/
   template <typename T, typename A>
   deque <T, A> ::deque(deque& rhs)
   {
      *this = rhs;
   }

   /*****************************************
    * DEQUE :: COPY-ASSIGN
    * Allocate the space for the elements and
    * call the copy constructor on each element
    ****************************************/
   template <typename T, typename A>
   deque <T, A>& deque <T, A> :: operator = (deque& rhs)
   {
      return *this;
   }

   /*****************************************
    * DEQUE :: PUSH_BACK
    * add an element to the back of the deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::push_back(const T& t)
   {}

   /*****************************************
    * DEQUE :: PUSH_BACK - move
    * add an element to the back of the deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::push_back(T&& t)
   {}

   /*****************************************
    * DEQUE :: PUSH_FRONT
    * add an element to the front of the deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::push_front(const T& t)
   {}

   /*****************************************
    * DEQUE :: PUSH_FRONT - move
    * add an element to the front of the deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::push_front(T&& t)
   {}

   /*****************************************
    * DEQUE :: CLEAR
    * Remove all the elements from a deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::clear()
   {}

   /*****************************************
    * DEQUE :: POP FRONT
    * Remove the front element from a deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::pop_front()
   {}

   /*****************************************
    * DEQUE :: POP BACK
    * Remove the back element from a deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::pop_back()
   {}

   /*****************************************
    * DEQUE :: REALLOCATE
    * Increase the size of the array of pointers
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::reallocate(int numBlocksNew)
   {
      // 1. Allocate a new array of pointers that is the requested size
      T** dataNew = (T**)alloc.allocate(numBlocksNew * sizeof(T*));

      // 2. Copy over the pointers, unwrapping as we go
      int ibNew = 0;
      for (int idOld = 0; idOld < numElements; idOld += numCells)
      {
         dataNew[ibNew] = data[ibFromID(idOld)];
         ibNew++;
      }

      // 3. Set all the block pointers to NULL when there are no blocks to point to
      while (ibNew < numBlocksNew)
         dataNew[ibNew] = nullptr;
      
      // 4. If the back element is in the front element's block, then move it
      if (numElements > 0
            && ibFromID(0) == ibFromID(numElements - 1)
            && icFromID(0) > icFromID(numElements - 1))
      {
         int ibFrontOld = ibFromID(0);
         int ibBackOld = ibFromID(numElements - 1);
         int ibBackNew = numElements / numCells;
         dataNew[ibBackNew] = new T[numCells];
         for (int ic = 0; ic <= icFromID(numElements - 1); ic++)
            dataNew[ibBackNew][ic] = std::move(data[ibBackOld][ic]);
      }

      // 5. Change the deque's member variables with the new values
      if (data)
         delete data;
         //alloc.deallocate((T*)data, numBlocks);
      data = dataNew;
      numBlocks = numBlocksNew;
      iaFront = iaFront % numCells;
   }




} // namespace custom
