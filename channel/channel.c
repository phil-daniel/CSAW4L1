
#include "channel.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

pthread_cond_t readyToRecieve = PTHREAD_COND_INITIALIZER;
pthread_cond_t sent = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct chan_t {
    int channel;
};

typedef struct chan_t chan_t;

chan_t *chan_create() {
    chan_t *chan = (chan_t *)malloc(sizeof(chan_t));
    return chan;
}

void chan_destroy(chan_t *chan) {
    free(chan);
}

void chan_send_int(chan_t *chan, int i) {
    // TODO: Sending logic

    // sender blocks until it gets signal from readyToRecieve condition variable
    // sender locks channel, puts value in, unlocks channel
    // sender sends signal on the sent condition variable

    // Does it wait for the reciever to say that it has recieved?

    pthread_cond_wait(&readyToRecieve, &lock);
    chan->channel = i;
    pthread_cond_signal(&sent);
    pthread_mutex_unlock(&lock);

    pthread_cond_wait(&readyToRecieve, &lock);
    pthread_mutex_unlock(&lock);

}

int chan_recv_int(chan_t *chan) {
    // TODO: Receiving logic

    // sends signal on the readyToRecieve condition variable
    // receiver blocks until it recieves a signal from the sent condition variable
    // once it recieves a signal, it means that the value has been sent
    // it will then copy the value in the channel and return it

    // do i need to loop the ready to recieve until we get the sent so that they are in sync
    int copier;
    pthread_cond_signal(&readyToRecieve);
    pthread_cond_wait(&sent, &lock);
    copier = chan->channel;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&readyToRecieve);
    return copier;
}