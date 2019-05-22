#include "SmartString.hpp"

using namespace std;

SmartString::SmartString(string* str) : str_(str), counter_(new unsigned int)
{
	(*counter_) = 1;
}


string* SmartString::get()
{
	return str_;
}

string* SmartString::operator->()
{
	return str_;
}

string& SmartString::operator*()
{
	return *str_;
}

SmartString::SmartString(const SmartString& other) : str_(other.str_), counter_(other.counter_)
{
	(*counter_)++;
}

SmartString& SmartString::operator=(const SmartString other)
{
	if (this != &other)
	{
		(*counter_)--;
		counter_ = other.counter_;
		str_ = other.str_;
		(*counter_)++;
	}

}

SmartString::~SmartString()
{
	(*counter_)--;
	
	if ((*counter_) == 0)
	{
		delete str_;
		delete counter_;
	}
}

	//Added getCount for visual testing purpose
unsigned int *SmartString::getCount()
{
	return counter_;
}
