#include "Vector.hpp"
#include <pthread.h>
#include "string.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

unsigned int userSleep = 0;

//Int mutex
pthread_mutex_t mutx = PTHREAD_MUTEX_INITIALIZER;

struct data
{
  unsigned int id_;
  Vector* vectorPtr_;
};

void* writer(void* argument)
{
	int hest;
	
  data *dataPtr = static_cast<data *>(argument);
  for (;;)
    {
		
	//Mutex Lock
	hest = pthread_mutex_lock(&mutx);
	//
				
      bool test = (*dataPtr).vectorPtr_->setAndTest((*dataPtr).id_);
      if (!test)
		{
	  cout << "Failed test at thread: " << (*dataPtr).id_ << endl;
		}
	  else
		{
	  cout << "PASSED" << endl;
		}
	 
	 //Mutex unlock
	 hest = pthread_mutex_unlock(&mutx);
	 //
	 
	 usleep(userSleep);
    }
  return nullptr;
}

int main(int argc, char *argv[])
{
  
  int threadsWanted = 0;
  
  //printf("How many threads do you want?\n");
  //scanf("%d", &threadsWanted);
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
