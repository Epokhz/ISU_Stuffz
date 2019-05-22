#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// stuct data
struct threadData{
    int threadId_;
};

void* print_message(void* data) {            //voidpointer = Trashcan
    threadData *tdata = static_cast<threadData *>(data);
    for(unsigned int i = 0;i < 10;i++){          // Write 10 times each
        std::cout << "ThreadID: "<< tdata->threadId_ << " Count: " << i<< std::endl;

        sleep(1); // sleep 1 sec
    } 
    return nullptr;
}
int main() {
    // create two stuck instances
    threadData *th1,*th2 = new threadData;
    // assign id's
    th1->threadId_ = 0;        //First thread ID
    th2->threadId_ = 1;        //Second thread ID

    // define threads
    pthread_t t1,t2;

    // create thread instance
    pthread_create(&t1, NULL, &print_message,  (void *)th1);  
    pthread_create(&t2, NULL, &print_message,  (void *)th2);
    // join threads to parent.
    pthread_join(t1,nullptr);
    pthread_join(t2,nullptr);

    return 0;
}
