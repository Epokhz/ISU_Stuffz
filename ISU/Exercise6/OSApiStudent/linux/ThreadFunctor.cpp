#include <osapi/Thread.hpp>

namespace osapi
{
  void* ThreadFunctor::threadMapper(void* thread)
  {
    /* Something is missing here - Determine what! */
    tf->threadDone_.signal();
    return NULL;
  }

}
