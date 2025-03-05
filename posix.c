#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int x = 2; // Shared global variable

void* routine(void* arg) {
    x += 5; // Modify shared variable
    sleep(2);
    printf("Value of x: %d\n", x);
    printf("Process Id: %d\n", getpid());
    return NULL;
}

void* routine2(void* arg) {
    sleep(2);
    printf("Value of x: %d\n", x);
    printf("Process Id: %d\n", getpid());
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, &routine, NULL);
    pthread_create(&t2, NULL, &routine2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
