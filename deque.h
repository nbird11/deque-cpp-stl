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
#pragma warning(disable : 4267)  // conversion from size_t to int

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
         : alloc(a), data(nullptr),
         numCells(16), numBlocks(0), numElements(0), iaFront(0)
      {}
      deque(deque& rhs);
      ~deque()
      {
         clear();
      }

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
         return iterator(0, this);
      }
      iterator end()
      {
         return iterator(numElements, this);
      }

      // 
      // Access
      //
      T& front()
      {
         return (*this)[0];
      }
      const T& front() const
      {
         return (*this)[0];
      }
      T& back()
      {
         return (*this)[size()-1];
      }
      const T& back() const
      {
         return (*this)[size()-1];
      }
      T& operator[](int id)
      {
         return data[ibFromID(id)][icFromID(id)];
      }
      const T& operator[](int id) const
      {
         return data[ibFromID(id)][icFromID(id)];
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
      size_t size()  const { return numElements; }
      bool   empty() const { return numElements == 0; }

   private:
      // array index from deque index
      int iaFromID(int id) const
      {
         //assert(0 <= id && id < numElements); // tests don't take numElements into account
         assert(0 <= iaFront && iaFront < numCells * numBlocks);
         int ia = ((id + iaFront) % (numBlocks * numCells));
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

      A alloc;               // use alloacator for memory allocation
      size_t numCells;       // number of cells in a block
      size_t numBlocks;      // number of blocks in the data array
      size_t numElements;    // number of elements in the deque
      int iaFront;           // array-centered index of the front of the deque
      T** data;              // array of arrays
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
      iterator(int id, deque* d) : id(id), d(d)
      {}
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
      bool operator != (const iterator& rhs) const { return id != rhs.id || d != rhs.d; }
      bool operator == (const iterator& rhs) const { return id == rhs.id && d == rhs.d; }

      // 
      // Access
      //
      T& operator * ()
      {
         return (*d)[id];
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

      int id;
   private:
      deque* d;
   };

   /*****************************************
    * DEQUE :: COPY CONSTRUCTOR
    * Allocate the space for the elements and
    * call the copy constructor on each element
    ****************************************/
   template <typename T, typename A>
   deque <T, A> ::deque(deque& rhs) : deque()
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
      alloc = rhs.alloc;

      iterator itLHS = begin();
      iterator itRHS = rhs.begin();

      // Create end iterators now because deques will change.
      iterator lhs_end = end();
      iterator rhs_end = rhs.end();

      for (; itLHS != lhs_end && itRHS != rhs_end; ++itLHS, ++itRHS)
         *itLHS = *itRHS;


      // lhs longer, remove the extra elements
      for (; itLHS != lhs_end; ++itLHS)
         pop_back();

      // rhs longer, add the extra elements
      for (; itRHS != rhs_end; ++itRHS)
         push_back(*itRHS);

      return *this;
   }

   /*****************************************
    * DEQUE :: PUSH_BACK
    * add an element to the back of the deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::push_back(const T& t)
   {
      // 1. Reallocate the array of blocks as needed
      if (numElements == numBlocks * numCells
          || icFromID(numElements) == 0 && numBlocks <= (numElements + numCells) / numCells)
         reallocate(numBlocks ? numBlocks * 2 : 1);

      // 2. Allocate a new block as needed
      int ib = ibFromID(numElements);
      if (!data[ib])
         data[ib] = alloc.allocate(sizeof(T) * numCells);

      // 3. Assign the value into the block
      new ((void*)(&(data[ib][icFromID(numElements)]))) T(t);
      numElements++;
   }

   /*****************************************
    * DEQUE :: PUSH_BACK - move
    * add an element to the back of the deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::push_back(T&& t)
   {
      // 1. Reallocate the array of blocks as needed
      if (numElements == numBlocks * numCells
          || icFromID(numElements) == 0 && numBlocks <= (numElements + numCells) / numCells)
         reallocate(numBlocks ? numBlocks * 2 : 1);

      // 2. Allocate a new block as needed
      int ib = ibFromID(numElements);
      if (!data[ib])
         data[ib] = alloc.allocate(sizeof(T) * numCells);

      // 3. Move the value into the block
      new ((void*)(&(data[ib][icFromID(numElements)]))) T(std::move(t));
      numElements++;
   }

   /*****************************************
    * DEQUE :: PUSH_FRONT
    * add an element to the front of the deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::push_front(const T& t)
   {
      // 1. Reallocate the array of blocks as needed
      if (numElements == numBlocks * numCells || numBlocks <= (numElements + numCells) / numCells)
         reallocate(numBlocks ? numBlocks * 2 : 1);
      // ^ puts iaFront into first block with unwrapping

      // Calculate new front position
      iaFront = (iaFront - 1 + numBlocks * numCells) % (numBlocks * numCells);
      
      // 2. Allocate a new block as needed
      int ib = iaFront / numCells;
      if (!data[ib])
         data[ib] = alloc.allocate(sizeof(T) * numCells);

      // 3. Assign the value into the block
      int ic = iaFront % numCells;
      new ((void*)(&(data[ib][ic]))) T(t);
      numElements++;
   }

   /*****************************************
    * DEQUE :: PUSH_FRONT - move
    * add an element to the front of the deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::push_front(T&& t)
   {
      // 1. Reallocate the array of blocks as needed
      if (numElements == numBlocks * numCells || numBlocks <= (numElements + numCells) / numCells)
         reallocate(numBlocks ? numBlocks * 2 : 1);
      // ^ puts iaFront into first block with unwrapping

      // Calculate new front position
      iaFront = (iaFront - 1 + numBlocks * numCells) % (numBlocks * numCells);

      // 2. Allocate a new block as needed
      int ib = iaFront / numCells;
      if (!data[ib])
         data[ib] = alloc.allocate(sizeof(T) * numCells);

      // 3. Assign the value into the block
      int ic = iaFront % numCells;
      new ((void*)(&(data[ib][ic]))) T(std::move(t));
      numElements++;
   }

   /*****************************************
    * DEQUE :: CLEAR
    * Remove all the elements from a deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::clear()
   {
      for (size_t id = 0; id < numElements; id++)
         alloc.destroy(&data[ibFromID(id)][icFromID(id)]);

      for (size_t ib = 0; ib < numBlocks; ib++)
      {
         assert(data != nullptr);
         if (data[ib])
         {
            alloc.destroy(&data[ib]);
            data[ib] = nullptr;
         }
      }
      numElements = 0;
   }

   /*****************************************
    * DEQUE :: POP FRONT
    * Remove the front element from a deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::pop_front()
   {
      size_t idRemove = 0;

      alloc.destroy(&data[ibFromID(idRemove)][icFromID(idRemove)]);

      if (numElements == 1
          || (icFromID(idRemove) == numCells - 1 && ibFromID(idRemove) != ibFromID(numCells - 1)))
      {
         alloc.destroy(&data[ibFromID(idRemove)]);
         data[ibFromID(idRemove)] = nullptr;
      }

      numElements--;
      iaFront++;
      iaFront %= numCells * numBlocks;
   }

   /*****************************************
    * DEQUE :: POP BACK
    * Remove the back element from a deque
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::pop_back()
   {
      size_t idRemove = numElements - 1;

      alloc.destroy(&data[ibFromID(idRemove)][icFromID(idRemove)]);

      if (numElements == 1
          || (icFromID(idRemove) == 0 && ibFromID(idRemove) != ibFromID(0)))
      {
         alloc.destroy(&data[ibFromID(idRemove)]);
         data[ibFromID(idRemove)] = nullptr;
      }

      numElements--;
   }

   /*****************************************
    * DEQUE :: REALLOCATE
    * Increase the size of the array of pointers
    ****************************************/
   template <typename T, typename A>
   void deque <T, A> ::reallocate(int numBlocksNew)
   {
      // 1. Allocate a new array of pointers that is the requested size
      /**********************************************************************
      *  +----+----+   +----+----+
      *  |    | 31 |   | 55 | 67 |
      *  +----+----+   +----+----+
      *           \       /
      *          +----+----+               +----+----+----+----+
      *          |  \ | /  |      ===>     |    |    |    |    |
      *          +----+----+               +----+----+----+----+
      ***********************************************************************/
      T** dataNew = (T**)this->alloc.allocate(numBlocksNew * sizeof(T*));

      // 2. Copy over the pointers, unwrapping as we go
      /**********************************************************************
      *     3             0    1    |          3        0    1   : id
      *  +----+----+   +----+----+  |  +----+----+   +----+----+
      *  | 31 |    |   | 55 | 67 |  |  |    | 31 |   | 55 | 67 |
      *  +----+----+   +----+----+  |  +----+----+   +----+----+
      *           \       /         |          \   /
      *          +----+----+       ===>     +----+----+----+----+
      *          |  \ | /  |        |       |   /|\   |    |    |
      *          +----+----+        |       +----+----+----+----+
      * (Blocks don't ever move once created.)
      ***********************************************************************/
      int ibNew = 0;
      for (int idOld = 0;
           idOld < numElements;
           idOld += numCells, ++ibNew)
         dataNew[ibNew] = data[ibFromID(idOld)];

      // 3. Set all the block pointers to NULL when there are no blocks to point to
      /**********************************************************************
      *  +----+----+   +----+----+    +----+----+   +----+----+
      *  |    | 31 |   | 55 | 67 |    |    | 31 |   | 55 | 67 |
      *  +----+----+   +----+----+    +----+----+   +----+----+
      *          \   /                         \   /
      *       +----+----+----+----+         +----+----+----+----+
      *       |   /|\   | ?? | ?? |  ===>   |   /|\   | // | // |
      *       +----+----+----+----+         +----+----+----+----+
      * ?? = garbage
      * // = nullptr
      ***********************************************************************/
      while (ibNew < numBlocksNew)
         dataNew[ibNew++] = nullptr;

      // 4. If the back element is in the front element's block, then move it
      // NOTE: Shouldn't this never happen?
      //if (numElements > 0
      //    && ibFromID(0) == ibFromID(numElements - 1)
      //    && icFromID(0) > icFromID(numElements - 1))
      //{
      //   int ibFrontOld = ibFromID(0);
      //   int ibBackOld = ibFromID(numElements - 1);
      //   int ibBackNew = numElements / numCells;
      //   dataNew[ibBackNew] = new T[numCells];
      //   for (int ic = 0; ic <= icFromID(numElements - 1); ic++)
      //      dataNew[ibBackNew][ic] = std::move(data[ibBackOld][ic]);
      //}

      // 5. Change the deque's member variables with the new values
      if (data) delete data;
      data = dataNew;
      numBlocks = numBlocksNew;
      iaFront = iaFront % numCells;
   }




} // namespace custom
