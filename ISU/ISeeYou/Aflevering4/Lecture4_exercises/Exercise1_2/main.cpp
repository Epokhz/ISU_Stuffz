#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int numberOfCars = 20;

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

//int for random pause in parkinglot
int randWaitTime; 

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
				
		// Car waits here for a while before wanting to drive out again
		// Random amount of pause, before trying to request exit
        randWaitTime=rand()%5+1;
        sleep(randWaitTime);
				
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
	pthread_t carThread[numberOfCars];
	pthread_t entryThread;
	pthread_t exitThread;
	
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
