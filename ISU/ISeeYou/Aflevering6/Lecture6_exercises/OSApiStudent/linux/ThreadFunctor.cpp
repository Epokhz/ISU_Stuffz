#include <osapi/Thread.hpp>

namespace osapi
{
  void* ThreadFunctor::threadMapper(void* thread)
  {
    /* Something is missing here - Determine what! */
   
    /* This is stolen from the slides from class (page 26) */
    ThreadFunctor* tf = static_cast<ThreadFunctor*>(thread);
    tf->run();
    
    tf->threadDone_.signal();
    return NULL;
  }
}
