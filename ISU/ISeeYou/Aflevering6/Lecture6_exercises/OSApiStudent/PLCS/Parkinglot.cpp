
#include "CarThread.hpp"
#include "EntryThread.hpp"
#include "ExitThread.hpp"

static int numberOfCars = 5;

int main(void)
{
    //Creating entry and exit thread
    EntryThread et(2);
    ExitThread xt(2);
    osapi::Thread entryT(&et);
    osapi::Thread exitT(&xt);
    entryT.start();
    exitT.start();
    
    //Creating car threads
    CarThread* cars[numberOfCars];
    osapi::Thread* carsT[numberOfCars];
    for(int x=0;x<numberOfCars;x++)
    {
        cars[x]  = new CarThread(x,5,et.getMsgQueue(),xt.getMsgQueue());
        carsT[x] = new osapi::Thread(cars[x]);
        carsT[x]->start();
    }
    entryT.join();
    return 0;
}
