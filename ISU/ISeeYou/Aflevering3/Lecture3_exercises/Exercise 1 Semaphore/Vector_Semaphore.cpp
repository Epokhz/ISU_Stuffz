#include "Vector.hpp"
#include <pthread.h>
#include "string.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>

using namespace std;

unsigned int userSleep = 0;

//Declerate semaphore
static sem_t Sema;

struct data
{
  unsigned int id_;
  Vector* vectorPtr_;
};

void* writer(void* argument)
{
  data *dataPtr = static_cast<data *>(argument);
  for (;;)
    {
		//Start semaphore
		sem_wait(&Sema);
				
      bool test = (*dataPtr).vectorPtr_->setAndTest((*dataPtr).id_);
      if (!test)
		{
	  cout << "Failed test at thread: " << (*dataPtr).id_ << endl;
		}
	  else
		{
	  cout << "PASSED" << endl;
		}
		
		//End semaphore
		sem_post(&Sema);
	 
	 usleep(userSleep);
    }
  return nullptr;
}

int main(int argc, char *argv[])
{  
  int threadsWanted = 0;

	//Initialize the semaphore
	sem_init(&Sema, 0, 1);
  
  cout << "How many threads do you want?" << endl;
  cin >> threadsWanted;
  cout << "How many microseconds do you want between tests?" << endl;
  cin >> userSleep;

  pthread_t threadAmount[threadsWanted];
  Vector *thisVector=new Vector();
  data dataArray[threadsWanted];
  
  for (int i=0; i<threadsWanted; i++)
    {
      dataArray[i].id_=i;
      dataArray[i].vectorPtr_=thisVector;
      pthread_create(&threadAmount[i], NULL, writer, (void *)&dataArray[i]);
    }

  for(int i=0; i<threadsWanted; i++)
    {
      pthread_join(threadAmount[i], nullptr);
    }
   return 0;
}
