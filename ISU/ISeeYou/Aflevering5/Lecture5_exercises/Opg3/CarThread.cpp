#include "CarThread.h"
#include "Messages.h"
#include "MsgQueue.h"
#include <iostream>
#include <unistd.h>
void* carFunc(void* messageQueueStruct)
{
    //Set ID 
    pthread_mutex_lock(&idMutex);
    ++carID;
    int carId_ = carID;
    pthread_mutex_unlock(&idMutex);


    messageQueues* queueStruct = static_cast<messageQueues*>(messageQueueStruct);
    MsgQueue* messageQueuePtr = static_cast<MsgQueue*>(queueStruct->messageQueue);
    MsgQueue* carMqPtr = static_cast<MsgQueue*>(queueStruct->carMq);
    MsgQueue carQ(10);    
    MsgQueue* carMsgQueuePtr = &carQ;
    
    
    carMsgQueuePtr->send(carStart);
    while (1) 
    {
        unsigned long id;
        //Receive message
        Message* msg= carMsgQueuePtr->receive(id);
        //Handle message
        carhandler(msg,id, carMsgQueuePtr, carMq, messageQueue, carId_);
        //Delete the dynamically allocated message created by the sender
        delete msg; 
    }
}
void* entryDoorController(void* messageQueueStruct)
{    
    messageQueues* queueStruct = static_cast<messageQueues*>(messageQueueStruct);
    MsgQueue* entryMsgQueuePtr = static_cast<MsgQueue*>(queueStruct->entryQ);
    while (1) 
    {
        unsigned long id;
        //Receive message
        Message* msg= entryMsgQueuePtr->receive(id);
        //Handle message
        entryhandler(msg,id, entryMsgQueuePtr);
        //Delete the dynamically allocated message created by the sender
        delete msg; 
    }
}
void* exitDoorController(void* messageQueueStruct)
{
    messageQueues* queueStruct = static_cast<messageQueues*>(messageQueueStruct);
    MsgQueue* exitMsgQueuePtr = static_cast<MsgQueue*>(queueStruct->exitQ);
    while (1) 
    {
        unsigned long id;
        //Receive message
        Message* msg= exitMsgQueuePtr->receive(id);
        //Handle message
        exithandler(msg, id, exitMsgQueuePtr);
        //Delete the dynamically allocated message created by the sender
        delete msg; 
    }
}
void* carhandler(Message* msg, unsigned long id,
                MsgQueue* carMsgQueue, MsgQueue* entryMsgQueue, MsgQueue* exitMsgQueue, int carID)
{
    switch(id)
    {
       case ID_CAR_SEND_OPEN:
       {
           carSendingEntryOpenReq(carMsgQueue, entryMsgQueue);
           break;
       } 
       
       case ID_ENTRY_DOOR_OPEN_CFM:
       {
          entryDoorOpenCfm* msgReceived = static_cast<entryDoorOpenCfm*>(msg);
           if(msgReceived->result)
           {
               carSendInCfm(entryMsgQueue, carID);
               carSendingExitOpenReq(carMsgQueue, exitMsgQueue);
           }
           else  //Stuck forever, could send itself a reset req
            std::cout << "Door not opened" << std::endl;
           break;
       }
       case ID_EXIT_OPEN_CFM:
       {
            //entryDoorOpenCfm* msgReceived = static_cast<entryDoorOpenCfm*>(msg);
            carSendOutCfm(exitMsgQueue, carID);
            sleep(1);
            //Restart yourself
            carMsgQueue->send(ID_CAR_SEND_OPEN);
        //    else 
        //     std::cout << "Exit door not opened" << std::endl;
            break;
       }
    }
}
void* entryhandler(Message* msg, unsigned long id, MsgQueue* entryMsgQueue)
{
    switch(id)
    {
        case ID_ENTRY_DOOR_OPEN_REQ:
        {
            entrySendingOpenCfm(msg, entryMsgQueue);
            break;
        }
        
        case ID_IN_CFM:
        {
            //Car is now in, decrement. 
            carsCurrentlyEntering--;
            if(carsCurrentlyEntering == 0)
            {
                std::cout << "Entry door closing" << std::endl;
            }
            break;
        }
    }
}
void* exithandler(Message* msg, unsigned long id, MsgQueue* exitMsgQueue)
{
    switch(id)
    {
        case ID_EXIT_OPEN_REQ:
        {
            exitSendingOpenCfm(msg);
            break;
        }
        case ID_OUT_CFM:
        {
            exitDoorClose();
            break;
        }
    }
}
void carSendingEntryOpenReq(MsgQueue* msgQueueCar, MsgQueue* msgQueueEntryDoor)
{
    //Create request
    entryDoorOpenReq* msgToBeSent = new entryDoorOpenReq;
    msgToBeSent->msgQueue_ = msgQueueCar;
    //Send it
    msgQueueEntryDoor->send(ID_ENTRY_DOOR_OPEN_REQ, msgToBeSent);
}
void carSendInCfm(MsgQueue* msgQueueEntryDoor, int carId)
{
    std::cout << "Car# " << carId << " drives into car park" << std::endl;
    //carsCurrentlyEntering--; Moved to  doors responsibility
    msgQueueEntryDoor->send(ID_IN_CFM);
    std::cout << "Car# " << carId <<  " parks" << std::endl;
    sleep(1);
}
void carSendingExitOpenReq(MsgQueue* msgQueueCar, MsgQueue* msgQueueExitDoor)
{
    //Create request
    exitOpenReq* msgToBeSent = new exitOpenReq;
    msgToBeSent->msgQueue_ = msgQueueCar;
    //Send it 
    msgQueueExitDoor->send(ID_EXIT_OPEN_REQ, msgToBeSent);
    //carsCurrentlyExiting++;
}
void carSendOutCfm(MsgQueue* msgQueueExitDoor, int carId)
{
    std::cout<< "Car# " << carId << " drives out of car park" << std::endl;
    msgQueueExitDoor->send(ID_OUT_CFM);
    //carsCurrentlyExiting--;
    
}
//Entry door functions 
void entrySendingOpenCfm(Message* message, MsgQueue* entryQueue)
{
    if (carsCurrentlyEntering == 0)
    {
        std::cout << "Opening entry door" << std::endl;
    }
    carsCurrentlyEntering++;
    entryDoorOpenReq* msgReceived = static_cast<entryDoorOpenReq*>(message);
    entryDoorOpenCfm* msgToBeSent = new entryDoorOpenCfm;
    msgToBeSent->result = 1;
    msgToBeSent->msgQueue_ = entryQueue;
    msgReceived->msgQueue_->send(ID_ENTRY_DOOR_OPEN_CFM, msgToBeSent);
}
// Exit door funtions 
void exitSendingOpenCfm(Message* msg)
{
    exitOpenReq* message = static_cast<exitOpenReq*>(msg);
    if (carsCurrentlyExiting == 0)
        std::cout << "Opening exit door" << std::endl;
    carsCurrentlyExiting++;
    message->msgQueue_->send(ID_EXIT_OPEN_CFM);
}
void exitDoorClose()
{
        carsCurrentlyExiting--;
    if (carsCurrentlyExiting == 0)
    {   
        std::cout << "Exit door closing" << std::endl; 
    }
}
