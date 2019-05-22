#include "Messages.h"
#include "MsgQueue.h"
#include <pthread.h>

#pragma once

static int carsCurrentlyEntering=0;
static int carsCurrentlyExiting=0;
static int carID=0;

static pthread_mutex_t idMutex = PTHREAD_MUTEX_INITIALIZER;

void* carFunc(void*);
void* entryDoorController(void*);
void* exitDoorController(void*);
void* carhandler(Message* msg, unsigned long id, MsgQueue* carMsgQueue, MsgQueue* entryMsgQueue, MsgQueue* exitMsgQueue, int carId);
void* entryhandler(Message* msg, unsigned long id, MsgQueue* entryMsgQueue);
void* exithandler(Message* msg, unsigned long id, MsgQueue* exitMsgQueue);

void carSendingEntryOpenReq(MsgQueue* msgQueueCar, MsgQueue* msgQueueEntryDoor);
void carSendInCfm(MsgQueue* msgQueueEntryDoor, int carId);
void carSendingExitOpenReq(MsgQueue* msgQueueCar, MsgQueue* msgQueueExitDoor);
void carSendOutCfm(MsgQueue* msgQueueExitDoor, int carId);
void entrySendingOpenCfm(Message* message, MsgQueue* entryQueue);
void exitSendingOpenCfm(Message* msg);
void exitDoorClose();
