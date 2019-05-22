
#include "Message.h"
#include "MsgQueue.h"

#pragma once

struct msgQs
{
    MsgQueue* entryQ;
    MsgQueue* exitQ;
};

enum
{
   openEntry,
   
};

struct entryDoorOpenReq : public Message
{
    MsgQueue* msgQueue_;
};

struct entryDoorOpenCfm : public Message 
{
   MsgQueue* msgQueue_;
   bool result; 
};

struct exitOpenReq : public Message 
{
    MsgQueue* msgQueue_;
};
