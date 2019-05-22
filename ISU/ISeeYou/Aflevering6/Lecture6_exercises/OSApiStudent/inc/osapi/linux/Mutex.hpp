#ifndef OSAPI_LINUX_MUTEX_HPP
#define OSAPI_LINUX_MUTEX_HPP

#include <pthread.h>
#include <osapi/Utility.hpp>

namespace osapi
{
	class Conditional; //Make friend class
	
	class Mutex : private Notcopyable
	{
	public:
		Mutex();
		~Mutex();
		void lock();
		void unlock();
	private:
		friend class Conditional;
		pthread_mutex_t mut_;
	};
}

#endif
