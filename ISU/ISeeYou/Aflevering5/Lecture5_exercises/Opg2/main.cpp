#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "MsgQueue.h"
#include "Message.h"
#include "point3D.h"

using namespace std;

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
	MsgQueue *messageQueue = (MsgQueue*)(arg);
	unsigned long id;
	while(true)
	{
		Message* msg = messageQueue -> receive(id);
		handleMsg (id, msg);
		delete msg;
	}
}

int main(int argc, char *argv[])
{
	unsigned long maxSize = 10;
	MsgQueue* messageQueue = new MsgQueue(maxSize);
	
	pthread_t senderThread;
	pthread_t receiverThread;
	
	pthread_create(&senderThread,   nullptr, Sender,    messageQueue);
	pthread_create(&receiverThread, nullptr, Receiver,  messageQueue);
	
	pthread_join(senderThread,   nullptr);
	pthread_join(receiverThread, nullptr);
	
	return 0;	
}
