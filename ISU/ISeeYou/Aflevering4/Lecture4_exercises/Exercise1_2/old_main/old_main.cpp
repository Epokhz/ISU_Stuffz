#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

using namespace std;

int numberOfCars = 2;

pthread_mutex_t entryMutex;
pthread_mutex_t exitMutex;
pthread_cond_t  entryCond;
pthread_cond_t  exitCond;

bool carWantIn = false;
bool carWantOut = false;
bool gateInOpen = false;
bool gateOutOpen = false;

//NEW STUFF
bool carInQueue = false;
bool carOutQueue = false;
int randWaitTime; 

void *car(void* arg)
{
	int carNumber = *((int*)arg);
	
	while(1)
		{
		pthread_mutex_lock(&entryMutex);
		
//		while(carInQueue == true) //Only one car can request entery at a time
//		{
//			cout << "Car " << carNumber << " is in queue" << endl;
//			pthread_cond_wait(&entryCond, &entryMutex);
//		}
//		carInQueue = true;
		
		cout << "Car " << carNumber << " requests entry" << endl;
		carWantIn = true;
		pthread_cond_signal(&entryCond);
		
		while(gateInOpen == false)
			{
			//cout << "Entry Gate is closed" << endl;
			pthread_cond_wait(&entryCond, &entryMutex);
			}
		cout << "Car " << carNumber << " enters" << endl;
		
		carWantIn = false;
		pthread_cond_signal(&entryCond);
		pthread_mutex_unlock(&entryMutex);
		
		// Random amount of pause, before trying to request exit
		randWaitTime=rand()%5+1;
		sleep(randWaitTime);
		
		pthread_mutex_lock(&exitMutex);
		
		while(carOutQueue == true) //Only one car can request exit at a time
		{
			//cout << "Car is in exit queue" << endl;
			pthread_cond_wait(&exitCond, &exitMutex);
		}
		carWantOut = true;
		pthread_cond_signal(&exitCond);
		cout << "Car " << carNumber << " requests exit" << endl;
		
		while(gateOutOpen == false)
			{
				//cout << "Exit Gate is closed" << endl;
				pthread_cond_wait(&exitCond, &exitMutex);
			}
		cout << "Car " << carNumber << " exits" << endl;
		carWantOut = false;
		pthread_cond_signal(&exitCond);
		pthread_mutex_unlock(&exitMutex);
		
		sleep(10);
 	}
}

void *guardEntry(void* arg)
{
	int entryNumber = *((int*)arg);
	while(entryNumber)
	{
		pthread_mutex_lock(&entryMutex);
		while(carWantIn == false)
			{
			cout << "No car wants in" << endl;
			pthread_cond_wait(&entryCond, &entryMutex);
			}
		
		cout << "Entry Gate is open" << endl;
		gateInOpen = true;
		//pthread_cond_signal(&entryCond);
		pthread_cond_broadcast(&entryCond);
		
		while(carWantIn == true)
			{
			cout << "A car wants in" << endl;
			pthread_cond_wait(&entryCond, &entryMutex);
			}
		cout << "Entry Gate is closed" << endl;
		gateInOpen = false;
		//pthread_cond_signal(&entryCond);
		pthread_cond_broadcast(&entryCond);
		
		carInQueue = false; //A new car can now request to enter
		
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
		
		while(carWantOut == false)
			{
			//cout << "No car wants out" << endl;
			pthread_cond_wait(&exitCond, &exitMutex);
			}
		cout << "Exit open" << endl;
		gateOutOpen = true;
		//pthread_cond_signal(&exitCond);
		pthread_cond_broadcast(&exitCond);
		
		while(carWantOut == true)
			{
			//cout << "A car wants out" << endl;
			pthread_cond_wait(&exitCond, &exitMutex);
			}
		cout << "Exit Gate is closed" << endl;
		gateOutOpen = false;
		//pthread_cond_signal(&exitCond);
		pthread_cond_broadcast(&exitCond);
		
		carOutQueue = false; //A new car can now request to exit
		
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
	for (int i = 0; i < numberOfCars; i++)
	{
		id[i] = i;
	}
	
	//Make the cars!
	for (int i = 0; i < numberOfCars; i++)
	{
		pthread_create(&carThread[i], NULL, car, &id[i]);
		cout << "Car "<< i << " created" << endl;
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
