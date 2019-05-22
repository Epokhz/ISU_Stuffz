#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "MsgQueue.h"
#include "Message.h"
#include "Messages.h"
#include "point3D.h"
#include <time.h>

using namespace std;

int numberOfCars = 3;



void* handleMsg(unsigned long id, Message* msg)
{
	switch(id) 
	{
	case carOpenRequest :
	{
		garageDoorOpenRequest* gtfo = (garageDoorOpenRequest*)(msg);		
		break;
	}
	case openEntryConfirm :
	{
		break;
	}
	case carIsIn :
	{
		
		break;
	}
	case carOpenExitRequest :
	{
		
		break;
	}
	case openExitConfirm :
	{
		
		break;
	}
	case carIsOut :
	{
		
		break;
	}
	default :
	{
		cout << "Unknown id\n" << endl;
		break;
	}
	}	
}

void* Car(void* arg)
{
	//ID
	pthread_mutex_lock()
	
	
	cout << "Cars wants in\n" << endl;
	MsgQueue* carMq = (MsgQueue*)(arg);   

	cout << "Cars sends open request\n" << endl;	
	carMq -> send(openEntry, nullptr);
	while(true)
	{	 
		unsigned long id;
		Message* msg = carMq->receive(id);
		handleMsg(id,msg);
		delete carMq;
	}
}

void* openHandler()
{
	
}

void* EntryGuard(void* arg)
{
	MsgQueue* mg = (MsgQueue*)(arg);
	while(true) 
	{
		unsigned long id = openEntry;
		Message* msg = mg->receive(id);
		handleMsg(id, msg);
		delete msg;
	}
}

void* garageDoorCloseControllerFunc(void* arg)
{
	
}

void* ExitGuard(void* arg)
{

}

int main(int argc, char *argv[])
{
	unsigned long maxSize = 10;
	MsgQueue messageQueue(maxSize);
	MsgQueue carMq(maxSize);
	
	MsgQs queueStruct(&messageQueue, &carMq);
	
	pthread_t carThread[numberOfCars];
	pthread_t entryThread;
	pthread_t exitThread;
	
    //Make the cars!
    for (int i = 0; i < numberOfCars; i++)
    {
        pthread_create(&carThread[i], NULL, Car, queueStruct);
    }

    //Make the rest!
    pthread_create(&entryThread, NULL, EntryGuard, queueStruct);
    pthread_create(&exitThread,  NULL, ExitGuard,  queueStruct);

    //Join cars
    for (int i = 0; i < numberOfCars; i++)
    {
        pthread_join(carThread[i], NULL);
    }

	//Join the rest
    pthread_join(entryThread, NULL);
    pthread_join(exitThread,  NULL);

	return 0;	
}
