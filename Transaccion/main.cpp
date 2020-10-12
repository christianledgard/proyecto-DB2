#include <iostream>
#include <pthread.h> 
#include <stdlib.h>
#include <semaphore.h>

using namespace std;
sem_t semaforo;


void* fthread_write(void *v)
{



  return NULL;
}

void* fthread_read(void *v)
{



  return NULL;
}

int main()
{
    pthread_t thread_write, thread_read;
    sem_init(&semaforo, 0, 1);
    pthread_create(&thread_write, NULL, fthread_write, NULL);
    pthread_create(&thread_read, NULL, fthread_read, NULL);


    pthread_join(thread_write,  NULL);
    pthread_join(thread_read,  NULL);

    return 0;
}
