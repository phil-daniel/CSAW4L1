#include <pthread.h>
#include <stdio.h>

void *hello_world(void *args) {
    int *n = args;
    printf("Hello from thread %d\n", *n);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    /*
    pthread_t thread;
    int n = 1;
    if (pthread_create(&thread, NULL, hello_world, &n)) {
        printf("Error creating thread\n");
    }
    if (pthread_join(thread, NULL)) {
        printf("Error joining thread\n");
    }
    */


    pthread_t threads[5];
    int threadsid[5];

    for (int n = 0; n < 5; n++) {
        threadsid[n] = n+1;
        if (pthread_create(&threads[n], NULL, hello_world, &threadsid[n])) {
            printf("Error creating thread\n");
        }
    }

    for (int n = 0; n < 5; n++) {
        if (pthread_join(threads[n], NULL)) {
            printf("Error joining thread\n");
        }
    }
}
