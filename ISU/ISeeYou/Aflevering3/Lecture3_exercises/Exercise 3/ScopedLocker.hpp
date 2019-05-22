#pragma once

#include <iostream>
#include <pthread.h>

using namespace std;

class ScopedLocker
{
public:
	ScopedLocker(pthread_mutex_t *mutex)
		{
		mutex_ = mutex;
		pthread_mutex_lock(mutex_);
		}
	~ScopedLocker() 
		{
			pthread_mutex_unlock(mutex_);
		}
private:	
	pthread_mutex_t *mutex_;
};
