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
	~SmartString();
	
private:
	string* str_;
	SmartString(const SmartString& other);
	SmartString& operator=(const SmartString other);
};

#endif
