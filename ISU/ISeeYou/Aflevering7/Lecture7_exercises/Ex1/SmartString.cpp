#include "SmartString.hpp"

using namespace std;

SmartString::SmartString(string* str) : str_(str) {}


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

SmartString::~SmartString()
{
	delete str_;
}
