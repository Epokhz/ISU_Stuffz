#pragma once

#include "Messages.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <osapi/Thread.hpp>
#include <osapi/ThreadFunctor.hpp>

class CarThread : public osapi::ThreadFunctor
{
public:
    CarThread(int carNumber, int mqSize, osapi::MsgQueue* entry, osapi::MsgQueue* exit) : mq_(mqSize), entryMq_(entry),exitMq_(exit)
    {
        carNumber_ = carNumber;
        running_ = true;
    }
    virtual void run()
    {
        gateMessage* firstEntry = new gateMessage();
        firstEntry->id = carNumber_;
        firstEntry->mq = &mq_;
        entryMq_->send(ASKING_FOR_ENTRY,firstEntry);
        while(running_)
        {
            unsigned long id;
            osapi::Message* m = mq_.receive(id);
            handleMsg(m,id);
            delete m;
        }
    }
    void handleMsg(osapi::Message* msg, unsigned long id)
    {
        switch(id)
        {
            case ENTRY_GRANTED:
            {
                //car is in carpark
                osapi::sleep(1000);
                gateMessage* exitPermission = new gateMessage();
                exitPermission->mq = &mq_;
                exitPermission->id = carNumber_;
                exitMq_->send(ASKING_FOR_EXIT,exitPermission);
                break;
            }
            case EXIT_GRANTED:
            {
                //Car is outside carpark
                osapi::sleep(1000);
                gateMessage* entryPermission = new gateMessage();
                entryPermission->id = carNumber_;
                entryPermission->mq = &mq_;
                entryMq_->send(ASKING_FOR_ENTRY,entryPermission);
                break;
            }
            default:
            {
                break;
            }
        }
    }
private:
    osapi::MsgQueue mq_;
    osapi::MsgQueue* entryMq_;
    osapi::MsgQueue* exitMq_;
    bool running_;
    int carNumber_;
};
