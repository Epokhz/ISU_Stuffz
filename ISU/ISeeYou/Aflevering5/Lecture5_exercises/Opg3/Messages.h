#include "Message.h"
#include "MsgQueue.h"

struct MsgQs
{
		MsgQueue* messageQueue;
		MsgQueue* carMq;
};

enum
{
		carStart,
		carOpenRequest,
		openEntryConfirm,
		carIsIn,
		carOpenExitRequest,
		openExitConfirm,
		carIsOut,		
};

struct garageENTRYDoorOpenRequest : public Message
{
		MsgQueue* mq_;
};

struct garageDoorOpenConf : public Message
{
		MsgQueue* mq_;
		bool result;
};

struct garageEXITDoorOpenRequest : public Message
{
		MsgQueue* mq_;
};
