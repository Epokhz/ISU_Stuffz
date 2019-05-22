#include "Message"
#include <queue>
#include <pthread.h>

using namespace std;

class MsgQueue
{
public: 
	MsgQueue(unsigned long maxSize);
	void send(unsigned long id, Message* msg = NULL);
	Message* receive(unsigned long& id);
	~MsgQueue();
private:
	struct Item
	{
		unsigned long int id_;
		Message* msg_;
	}
	
	queue_<Item*> container_;
	unsigned long maxSize_;	
	
	pthread_cond_t sendCond_ 		= PTHREAD_COND_INITIALIZER;
	pthread_cond_t receiveCond_		= PTHREAD_COND_INITIALIZER;
	pthread_mutex_t sendMutex_		= PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t receiveMutex_	= PTHREAD_MUTEX_INITIALIZER;
	
	};
	

