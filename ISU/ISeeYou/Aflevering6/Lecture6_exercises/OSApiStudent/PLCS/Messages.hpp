#pragma once

#include <osapi/Message.hpp>
#include <osapi/MsgQueue.hpp>
#include <string>

struct gateMessage : public osapi::Message
{
    osapi::MsgQueue* mq;
    int id;
};

enum
{
    ASKING_FOR_ENTRY,
    ENTRY_GRANTED,
    ASKING_FOR_EXIT,
    EXIT_GRANTED,
    LOG_EVENT,
};
