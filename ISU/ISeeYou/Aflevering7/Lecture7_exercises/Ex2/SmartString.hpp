#ifndef SMARTSTRING_H
#define SMARTSTRING_H

#include <string>
#include <iostream>
using namespace std;

class SmartString
{ 
public:
	SmartString(string* str);
	string* get();
	string* operator->();
	string& operator*();
	SmartString(const SmartString& other);
	SmartString& operator=(const SmartString other);
	~SmartString();
	
	unsigned int* getCount(); // Added for testing purpose
	
private:
	string* str_;
	unsigned int* counter_;
};

#endif
