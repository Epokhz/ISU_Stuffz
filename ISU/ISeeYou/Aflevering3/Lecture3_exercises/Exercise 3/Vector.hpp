#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <pthread.h>
#include "ScopedLocker.hpp"
//=======================================================
// Class: Vector
// contains a size_-size vector of integers.
// Use the function setAndTest to set all elements
// of the vector to a certain value and then test that
// the value is indeed correctly set
//=======================================================
class Vector
{
public:
   Vector(unsigned int size = 10000) : size_(size)
      { 
         //init mutex
         mutex_ = PTHREAD_MUTEX_INITIALIZER;
         
         vector_ = new int[size_];
         set(0);
      }
   ~Vector()
      {
         delete[] vector_;
      }
   bool setAndTest(int n)
      {
         //call constructor
         ScopedLocker lockedScope(&mutex_);
         set(n);
         return test(n);
         //auto-destructer when leaving scope
      }
private:
   void set(int n)
      {
         for(unsigned int i=0; i<size_; i++) vector_[i] = n;
      }
   bool test(int n)
      {
         for(unsigned int i=0; i<size_; i++) if(vector_[i] != n) return false;
         return true;
      }
   int*           vector_;
   unsigned int   size_;
   //
   pthread_mutex_t mutex_;
};
#endif
