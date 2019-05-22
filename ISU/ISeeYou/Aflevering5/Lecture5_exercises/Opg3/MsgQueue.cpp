#include "MsgQueue.h"
#include <iostream>

using namespace std;

MsgQueue::MsgQueue(unsigned long maxSize)
{
	maxSize_ = maxSize;
	
	//initiate Mutexes and Conditions
	pthread_mutex_init(&sendMutex_,     nullptr);
	pthread_mutex_init(&receiveMutex_,  nullptr);
	pthread_cond_init (&sendCond_,      nullptr);
	pthread_cond_init (&receiveCond_,   nullptr);
}

void MsgQueue::send(unsigned long id, Message* msg)
{
	//lock sendMutex_
	pthread_mutex_lock(&sendMutex_);
	
	while (container_.size() >= maxSize_)
	{
		pthread_cond_wait(&sendCond_, &sendMutex_);
	}
	
	//Create item and values
	Item* itemPtr = new Item;
	itemPtr->id_ = id;
	itemPtr->msg_ = msg;
	
	//Put item in queue
	container_.push(itemPtr);
	
	//Signal receiver
	pthread_cond_signal(&receiveCond_);
	
	//unlock sendMutex_
	pthread_mutex_unlock(&sendMutex_);
	
	// cout << (container_.back()) << endl; 
		
	//cout << "Sending ID " << id << " to receiver" << endl;
}

Message* MsgQueue::receive(unsigned long id)
{
	//lock receiveMutex_
	pthread_mutex_lock(&receiveMutex_);
	
		while (container_.size() == 0)
	{
		pthread_cond_wait(&receiveCond_, &receiveMutex_);
	}
	
	// Receive item and save in new variables
	Item* resultPtr = container_.front();
	id = resultPtr->id_;
	Message* msgPtr = resultPtr->msg_;
	
	//Pop that shit
	container_.pop();
	
	//Delete item to clean up
	delete resultPtr;
	
	//Signal sender
	pthread_cond_signal(&sendCond_);
	
	//Unlock receiveMutex_
	pthread_mutex_unlock(&receiveMutex_);
	
	//cout << "Received ID " << id << " from sender" <<endl;
	
	return msgPtr;
}

MsgQueue::~MsgQueue()
{
	
}
