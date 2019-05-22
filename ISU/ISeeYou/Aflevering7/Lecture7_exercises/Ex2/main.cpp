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
	
	
	cout << "ss counter says: " << *ss.getCount() << endl;

	{
			cout << "Assignment scope \n Copying ss to ss2" << endl;
			SmartString ss2 = ss; // Copy constructor
			
			cout << "ss2 writes: '" << *ss2 << "'" <<endl;
			cout << "ss counter counts: " << *ss.getCount() << endl;
			cout << "ss2 counter counts: " << *ss2.getCount() << endl;
	}
	
	cout << "Left assignment scope" << endl; 
	cout << "ss counter counts: " << *ss.getCount() << endl;

}
