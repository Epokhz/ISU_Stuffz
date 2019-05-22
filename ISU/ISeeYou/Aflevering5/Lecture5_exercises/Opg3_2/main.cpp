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

void* printPoint3DVars(Message* msg)
{
        point3D* p = (point3D*)(msg);
        cout << "RECEIVE THIS(" << p->x << ", " << p->y << ", " << p->z << ")" << endl;
}

void* handleMsg(unsigned long id, Message* msg)
{
	switch(id) 
	{
	case p3d :
		printPoint3DVars(msg);
		break;
	default :
		cout << "Unknown id\n" << endl;
		break;
	}	
}

void* Sender(void* arg)
{
	MsgQueue* messageQueue = (MsgQueue*)(arg);
	
	while(true) {
		point3D* point3d = new point3D;
		point3d->x = rand() %10+1;
		point3d->y = rand() %10+1;
		point3d->z = rand() %10+1;
		messageQueue->send(p3d, point3d);
		
		cout << "SEND THIS: (" << point3d->x << ", " << point3d->y << ", " << point3d->z << ")" << endl;
		
		sleep(1); //Every second or so
	}
}

void* Receiver(void* arg)
{
	msgQs* msgstruct = static_cast<msgQs>(arg);
	
	MsgQueue *messageQueue = (MsgQueue*)(arg);
	unsigned long id;
	while(true)
	{
		Message* msg = messageQueue -> receive(id);
		handleMsg (id, msg);
		delete msg;
	}
}

void* Car(void* arg)
{
	msgQs* Qsptr = static_cast<msgQs*>(arg);
	MsgQueue* entryptr = static_cast<MsgQueue>entryQ;
	MsgQueue* exitptr = static_cast<MsgQueue>exitQ;
	cout << "Cars wants in\n" << endl;
  	cout << "Cars sends open request\n" << endl;	
	entryptr -> send(openEntry, entryptr);
	while(true)
	{	 
		unsigned long id;
		Message* msg =  entryptr->receive(id);
		handleMsg(id,msg);
		delete carMq;
	}
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

void* EntryGuard(void* arg)
{

}

void* ExitGuard(void* arg)
{

}

int main(int argc, char *argv[])
{
	unsigned long maxSize = 10;
	MsgQueue* entryQ = new MsgQueue(maxSize);
	MsgQueue* exitQ = new MsgQueue(maxSize);
	msgQs Qs= {&entryQ, exitQ};
	int i;
	
	pthread_t senderThread;
	pthread_t receiverThread;
	pthread_t cars[maxSize]
	
	for(i=0; i>9; i++)
	{ pthread_create(&cars[i], nullptr, Car, &Qs);}
	
	pthread_create(&senderThread,   nullptr, Sender,    Qs);
	pthread_create(&receiverThread, nullptr, Receiver, Qs);
	
	pthread_join(senderThread,   nullptr);
	pthread_join(receiverThread, nullptr);
	
	return 0;	
}
