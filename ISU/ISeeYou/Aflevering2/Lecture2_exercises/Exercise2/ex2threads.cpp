#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

unsigned int shared = 0;

void* incrementer(void* data)
{
  while(1)
    {
      unsigned int* shared = static_cast<unsigned int*>(data);
      (*shared)++;
      //printf("test: %d \n", *(shared));
      sleep(1);
    }
}

void* reader(void* data)
{
  while(1)
    {
      unsigned int* shared = static_cast<unsigned int*>(data);

      printf("Counter is: %d \n", *(shared));
      sleep(1);
    }
}

int main()
{
  // unsigned int shared = 0;

    // define threads
    pthread_t t1,t2;

    // create thread instance
              //pthread_create(&t2, NULL, reader, &shared);
    pthread_create(&t1, NULL, incrementer, &shared);  
    pthread_create(&t2, NULL, reader, &shared);
 
    // join threads (Wait until threads are done)
    pthread_join(t1,nullptr);
    pthread_join(t2,nullptr);

    return 0;
}
