#pragma once

#include <osapi/Thread.hpp>
#include <osapi/ThreadFunctor.hpp>
#include "Messages.hpp"
#include <iostream>

class ExitThread : public osapi::ThreadFunctor
{
public:
    ExitThread(int mqSize) : mq_(mqSize)
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
            case ASKING_FOR_EXIT:
            {
                gateMessage* exitPermission = static_cast<gateMessage*>(msg);
                std::cout<<"Car "<<exitPermission->id<<" is asking for exit"<<std::endl;
                exitPermission->mq->send(EXIT_GRANTED,NULL);
                std::cout<<"Car "<<exitPermission->id<<" is exiting carpark"<<std::endl;
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
