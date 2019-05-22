#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

using namespace std;

int numberOfCars = 10;

static pthread_mutex_t entryMutex;
static pthread_mutex_t exitMutex;
static pthread_cond_t  entryCond;
static pthread_cond_t  exitCond;

static pthread_cond_t  capacityCond; //NEW
int capacity     = 0;
int maxCapacity  = 4;

int carWantIn   = 0;
int carWantOut  = 0;
int gateInOpen  = 0;
int gateOutOpen = 0;

//NEW STUFF
int carInQueue  = 0;
int carOutQueue = 0;
int randWaitTime; 

void *car(void* arg)
{
	int carNumber = *((int*)arg);
	
	while(1)
		{
		pthread_mutex_lock(&entryMutex);
				
		while(carInQueue == 1) //Only one car can request entery at a time
		{
			cout << "Car " << carNumber << " is in queue" << endl;
			pthread_cond_wait(&entryCond, &entryMutex);
		}
		carInQueue = 1;

		while(capacity == maxCapacity) //NEW
		{
			cout << "No more room!" << endl;
			pthread_cond_wait(&capacityCond, &entryMutex);
		}
		
		cout << "Current Capacity is " << capacity << endl; //NEW
		
		cout << "Car " << carNumber << " requests entry" << endl;
		carWantIn = 1;
		pthread_cond_signal(&entryCond);
		
		while(gateInOpen == 0)
			{
			//cout << "Entry Gate is closed" << endl;
			pthread_cond_wait(&entryCond, &entryMutex);
			}
		cout << "Car " << carNumber << " enters" << endl;
		
		capacity++; // NEW
		
		carWantIn = 0;
		pthread_cond_signal(&entryCond);
		pthread_mutex_unlock(&entryMutex);
		
		//Random amount of pause, before trying to request exit
		//randWaitTime=rand()%5+1;
		//sleep(randWaitTime);
		sleep(2);
		
		pthread_mutex_lock(&exitMutex);
		
		while(carOutQueue == 1) //Only one car can request exit at a time
		{
			//cout << "Car is in exit queue" << endl;
			pthread_cond_wait(&exitCond, &exitMutex);
		}
		carWantOut = 1;
		pthread_cond_signal(&exitCond);
		cout << "Car " << carNumber << " requests exit" << endl;
		
		while(gateOutOpen == 0)
			{
				//cout << "Exit Gate is closed" << endl;
				pthread_cond_wait(&exitCond, &exitMutex);
			}
		cout << "Car " << carNumber << " exits" << endl;
		
		capacity--; //NEW
		pthread_cond_signal(&capacityCond);
		
		cout << "Current Capacity is " << capacity << endl; //NEW
		
		carWantOut = 0;
		pthread_cond_signal(&exitCond);
		pthread_mutex_unlock(&exitMutex);
		
		sleep(30);
 	}
}

void *guardEntry(void* arg)
{
	int entryNumber = *((int*)arg);
	while(entryNumber)
	{
		pthread_mutex_lock(&entryMutex);
		while(carWantIn == 0)
			{
			//cout << "No car wants in" << endl;
			pthread_cond_wait(&entryCond, &entryMutex);
			}
			
		cout << "Entry Gate is open" << endl;
		gateInOpen = 1;
		pthread_cond_broadcast(&entryCond);
		
		while(carWantIn == 1)
			{
			//cout << "A car wants in" << endl;
			pthread_cond_wait(&entryCond, &entryMutex);
			}
		cout << "Entry Gate is closed" << endl;
		gateInOpen = 0;
		pthread_cond_broadcast(&entryCond);
		
		carInQueue = 0; //A new car can now request to enter
		
		pthread_mutex_unlock(&entryMutex);
	}
	return nullptr;
}

void *guardExit(void* arg)
{
	int exitNumber = *((int*)arg);
	while(exitNumber)
	{
		pthread_mutex_lock(&exitMutex);
		
		while(carWantOut == 0)
			{
			//cout << "No car wants out" << endl;
			pthread_cond_wait(&exitCond, &exitMutex);
			}
		cout << "Exit open" << endl;
		gateOutOpen = 1;
		pthread_cond_broadcast(&exitCond);
		
		while(carWantOut == 1)
			{
			//cout << "A car wants out" << endl;
			pthread_cond_wait(&exitCond, &exitMutex);
			}
		cout << "Exit Gate is closed" << endl;
		gateOutOpen = 0;
		pthread_cond_broadcast(&exitCond);
		
		carOutQueue = 0; //A new car can now request to exit
				
		pthread_mutex_unlock(&exitMutex);
	}
	return nullptr;
}

int main(int argc, char *argv[])
{
	//initiate Mutexes and Conditions
	pthread_mutex_init(&entryMutex, nullptr);
	pthread_mutex_init(&exitMutex,  nullptr);
	pthread_cond_init (&entryCond,  nullptr);
	pthread_cond_init (&exitCond,   nullptr);
	
	//make threads
	pthread_t carThread[numberOfCars];
	pthread_t entryThread;
	pthread_t exitThread;
	
	//int *car_ = new int (1);
	int *entry_ = new int (1); 
	int *exit_  = new int (1);
	
	//Make an amount of IDs
	int id[numberOfCars];
	for (int i = 0; i<numberOfCars;i++)
	{
		id[i] = i;
	}
	
	//Make the cars!
	for (int i = 0; i < numberOfCars; i++)
	{
		pthread_create(&carThread[i], NULL, car, &id[i]);
	}
	
	//Make the rest!
	pthread_create(&entryThread, NULL, guardEntry, entry_);
	pthread_create(&exitThread,  NULL, guardExit,  exit_ );

	//Join everything
	for (int i = 0; i < numberOfCars; i++)
	{
		pthread_join(carThread[i], NULL);
	}
	
	pthread_join(entryThread, NULL);
	pthread_join(exitThread,  NULL);
	
	return 0;	
}
