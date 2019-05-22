#ifndef OSAPI_LINUX_UTILITY_CPP
#define OSAPI_LINUX_UTILITY_CPP

#include <osapi/Utility.hpp>
#include <unistd.h>

namespace osapi
{
	void sleep(unsigned long msecs)
	{
		usleep(msecs*1000); //Convert from microsec to milisec
	}	
}

#endif
