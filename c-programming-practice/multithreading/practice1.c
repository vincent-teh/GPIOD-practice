#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *print_message(void *ptr);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 2;

int main(){
    pthread_t thread1, thread2;
    char *message1 = "thread1";
    char *message2 = "thread2";
    int iret1, iret2;

    iret1 = pthread_create( &thread1, NULL, &print_message, (void*) message1);
    iret2 = pthread_create( &thread1, NULL, &print_message, (void*) message2);

    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);

    printf("Thread 1 return %d\n", iret1);
    printf("Thread 2 return %d\n", iret2);
    printf("%d\n", counter);
    return 0;
}

void *print_message(void *ptr)
{
    char *message;
    message = (char *) ptr;
    // pthread_mutex_lock( &mutex1 );
    counter--;
    printf("This is %s, counter: %d \n", message, counter);
    // pthread_mutex_unlock( &mutex1 );
}
