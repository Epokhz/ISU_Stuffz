#pragma once

#include <osapi/Thread.hpp>
#include <osapi/ThreadFunctor.hpp>
#include "Messages.hpp"
#include <iostream>

class EntryThread : public osapi::ThreadFunctor
{
public:
    EntryThread(int mqSize) : mq_(mqSize)
    {
        running_ = true;
    }
    virtual void run()
    {
        while(running_)
        {
            unsigned long id;
            osapi::Message* m = mq_.receive(id);
            handleMsg(m,id);
            delete m;
        }
    }
    void handleMsg(osapi::Message* msg,unsigned long id)
    {
        switch(id)
        {
            case ASKING_FOR_ENTRY:
            {
                gateMessage* entryPermission = static_cast<gateMessage*>(msg);
                std::cout<<"Car "<<entryPermission->id<<" is asking for entry"<<std::endl;
                entryPermission->mq->send(ENTRY_GRANTED,NULL);
                std::cout<<"Car "<<entryPermission->id<<" is entering the carpark"<<std::endl;
                break;
            }
            default:
            {
                break;
            }
        }
    }
    osapi::MsgQueue* getMsgQueue()
    {
        return &mq_;
    }
private:
    osapi::MsgQueue mq_;
    bool running_;
};
