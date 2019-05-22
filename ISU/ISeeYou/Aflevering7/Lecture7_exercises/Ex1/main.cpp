#include <iostream>
#include "SmartString.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	SmartString ss(new string("Hello world"));
	cout << "String length: " << ss->length() << endl;
	
	if (ss->find("world") != string::npos)
		cout << "The 'ss' string does contain 'world'" << endl;
	else
		cout << "The 'ss' string does NOT contain 'world'" << endl;
	
	//SmartString ss2 = ss; 
}
