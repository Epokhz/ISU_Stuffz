#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int numberOfCars = 1;

//initiate mutex
static pthread_mutex_t Mutx;

//initiate conditions
static pthread_cond_t  entryCond;
static pthread_cond_t  exitCond;

//Car states
bool carWantIn   = false;
bool carWantOut  = false;

//Door states
bool gateInOpen  = false;
bool gateOutOpen = false;

void* car(void* arg)
{
	int carNumber = *((int*)arg);
	
	while(1)
		{
		cout << "Car " << carNumber << " requests entry" << endl;
		
		//lock mutex
		pthread_mutex_lock(&Mutx);
		
		carWantIn = true;
		
		pthread_cond_signal(&entryCond);
		
		//wait here as long as the entry door is closed
		while(!gateInOpen)
			{
			cout << "Entry is closed" << endl;
			pthread_cond_wait(&entryCond, &Mutx);
			}
		
		cout << "Car " << carNumber << " enters" << endl;
		
		carWantIn = false;
		
		pthread_cond_signal(&entryCond);
		
		//unlock mutex
		pthread_mutex_unlock(&Mutx);
				
		sleep(2); // Car waits here for a while before wanting to drive out again
		
		//lock mutex
		pthread_mutex_lock(&Mutx);
		
		carWantOut = true;
		
		pthread_cond_signal(&exitCond);
		
		cout << "Car " << carNumber << " requests exit" << endl;
		
		//wait here until exit door is open
		while(!gateOutOpen)
			{
				pthread_cond_wait(&exitCond, &Mutx);
			}
			cout << "Car " << carNumber << " exits" << endl;
		
		carWantOut = false;
		pthread_cond_signal(&exitCond);

		pthread_mutex_unlock(&Mutx);
		
		//wait until repeat
		sleep(2);
 	}
 	return nullptr;
}

void* guardEntry(void* arg)
{
	while(1)
	{
		//lock mutex
		pthread_mutex_lock(&Mutx);
		
		//wait here while no car wants to drive in
		while(!carWantIn)
			{
			pthread_cond_wait(&entryCond, &Mutx);
			}
		
		gateInOpen = true;
		
		pthread_cond_broadcast(&entryCond);
		cout << "Entry open" << endl;
		
		//wait here while a car is waiting to get in
		while(carWantIn)
			{
			pthread_cond_wait(&entryCond, &Mutx);
			}
		
		gateInOpen = false;
		
		pthread_cond_broadcast(&entryCond);
		cout << "Entry closed" << endl;
		
		//unlock mutex
		pthread_mutex_unlock(&Mutx);
	}
	return nullptr;
}

void* guardExit(void* arg)
{
	while(1)
	{
		//lock mutex
		pthread_mutex_lock(&Mutx);
		
		//wait here while no car wants to exit
		while(!carWantOut)
			{
			pthread_cond_wait(&exitCond, &Mutx);
			}
		
		gateOutOpen = true;
		
		pthread_cond_broadcast(&exitCond);
		cout << "Exit open" << endl;
		
		//wait here while a car wants to exit
		while(carWantOut)
			{
			pthread_cond_wait(&exitCond, &Mutx);
			}
			
		gateOutOpen = false;
		
		pthread_cond_broadcast(&exitCond);
		cout << "Exit closed" << endl;
		
		//unlock mutex
		pthread_mutex_unlock(&Mutx);
	}
	return nullptr;
}

int main(int argc, char *argv[])
{
	//initiate mutex
	pthread_mutex_init(&Mutx, nullptr);
	
	//initiate conditions
	pthread_cond_init (&entryCond,  nullptr);
	pthread_cond_init (&exitCond,   nullptr);
	
	//make threads
	pthread_t carThread;
	pthread_t entryThread;
	pthread_t exitThread;
	
	int *car_   = new int (1);
	int *entry_ = new int (1); 
	int *exit_  = new int (1);
	
	//create threads
	pthread_create(&carThread,   NULL, car, car_);
	pthread_create(&entryThread, NULL, guardEntry, entry_);
	pthread_create(&exitThread,  NULL, guardExit, exit_);
	
	//join threads
	pthread_join(carThread,   nullptr);
	pthread_join(entryThread, nullptr);
	pthread_join(exitThread,  nullptr);
	
	return 0;	
}
