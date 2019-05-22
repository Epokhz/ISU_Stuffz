#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int numberOfCars = 1;

//Init mutex's
//pthread_mutex_t entryMutex;
//pthread_mutex_t exitMutex;
static pthread_mutex_t Mutx;
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
		
		//pthread_mutex_lock(&entryMutex);
		pthread_mutex_lock(&Mutx);
		
		carWantIn = true;
		pthread_cond_signal(&entryCond);
		
		while(!gateInOpen)
			{
				cout << "Entry is closed" << endl;
			//pthread_cond_wait(&entryCond, &entryMutex);
			pthread_cond_wait(&entryCond, &Mutx);
			}
			cout << "Car " << carNumber << " enters" << endl;
		
		carWantIn = false;
		
		pthread_cond_signal(&entryCond);
		//pthread_mutex_unlock(&entryMutex);
		pthread_mutex_unlock(&Mutx);
				
		sleep(2); // Car waits here for a while
		
		//pthread_mutex_lock(&exitMutex);
		pthread_mutex_lock(&Mutx);
		
		carWantOut = true;
		pthread_cond_signal(&exitCond);
			cout << "Car " << carNumber << " requests exit" << endl;
		
		while(!gateOutOpen)
			{
				//pthread_cond_wait(&exitCond, &exitMutex);
				pthread_cond_wait(&exitCond, &Mutx);
			}
			cout << "Car " << carNumber << " exits" << endl;
		
		carWantOut = false;
		pthread_cond_signal(&exitCond);
		
		//pthread_mutex_unlock(&exitMutex);
		pthread_mutex_unlock(&Mutx);
		
		//wait until repeat
		sleep(2);
 	}
 	return nullptr;
}

void* guardEntry(void* arg)
{
	//int entryNumber = *((int*)arg);
	//while(entryNumber)
	while(1)
	{
		//pthread_mutex_lock(&entryMutex);
		pthread_mutex_lock(&Mutx);
		
		//No car is waiting
		while(!carWantIn)
			{
			//pthread_cond_wait(&entryCond, &entryMutex);
			pthread_cond_wait(&entryCond, &Mutx);
			}
		
		gateInOpen = true;
		pthread_cond_broadcast(&entryCond);
		cout << "Entry open" << endl;
		
		//A car is waiting
		while(carWantIn)
			{
			//pthread_cond_wait(&entryCond, &entryMutex);
			pthread_cond_wait(&entryCond, &Mutx);
			}
		gateInOpen = false;
		pthread_cond_broadcast(&entryCond);
		cout << "Entry closed" << endl;
		
		//pthread_mutex_unlock(&entryMutex);
		pthread_mutex_unlock(&Mutx);
	}
	return nullptr;
}

void* guardExit(void* arg)
{
	//int exitNumber = *((int*)arg);
	//while(exitNumber)
	while(1)
	{
		//pthread_mutex_lock(&exitMutex);
		pthread_mutex_lock(&Mutx);
		
		while(!carWantOut)
			{
			//pthread_cond_wait(&exitCond, &exitMutex);
			pthread_cond_wait(&exitCond, &Mutx);
			}
		gateOutOpen = true;
		pthread_cond_broadcast(&exitCond);
		cout << "Exit open" << endl;
		
		while(carWantOut)
			{
			//pthread_cond_wait(&exitCond, &exitMutex);
			pthread_cond_wait(&exitCond, &Mutx);
			}
		gateOutOpen = false;
		pthread_cond_broadcast(&exitCond);
		cout << "Exit closed" << endl;
		
		//pthread_mutex_unlock(&exitMutex);
		pthread_mutex_unlock(&Mutx);
	}
	return nullptr;
}

int main(int argc, char *argv[])
{
	//initiate Mutexes and Conditions
	//pthread_mutex_init(&entryMutex, nullptr);
	//pthread_mutex_init(&exitMutex,  nullptr);
	pthread_mutex_init(&Mutx, nullptr);
	pthread_cond_init (&entryCond,  nullptr);
	pthread_cond_init (&exitCond,   nullptr);
	
	pthread_t carThread;
	pthread_t entryThread;
	pthread_t exitThread;
	
	int *car_   = new int (1);
	int *entry_ = new int (1); 
	int *exit_  = new int (1);
	
	pthread_create(&carThread,   NULL, car, car_);
	pthread_create(&entryThread, NULL, guardEntry, entry_);
	pthread_create(&exitThread,  NULL, guardExit, exit_);
	
	pthread_join(carThread,   nullptr);
	pthread_join(entryThread, nullptr);
	pthread_join(exitThread,  nullptr);
	
	return 0;	
}
