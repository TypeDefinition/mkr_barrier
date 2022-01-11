#ifndef MKR_BARRIER_H
#define MKR_BARRIER_H

// C Standard Library Includes
#include <semaphore.h>

// Barrier implementation.
// Not allowed to use pthread_barrier?
// Fine! I'll make my own barrier, with blackjack and hookers! In fact, forget the barrier!
typedef struct {
    int num_threads;
    int counter;
    sem_t entrance;
    sem_t counter_mutex;
    sem_t gate;
} mkr_barrier_t;

void mkr_barrier_init(mkr_barrier_t* barrier, int num_threads) {
    barrier->num_threads = num_threads;
    barrier->counter = 0;
    sem_init(&barrier->entrance, 0, num_threads);
    sem_init(&barrier->counter_mutex, 0, 1);
    sem_init(&barrier->gate, 0, 0);
}

void mkr_barrier_destroy(mkr_barrier_t* barrier) {
    sem_destroy(&barrier->entrance);
    sem_destroy(&barrier->counter_mutex);
    sem_destroy(&barrier->gate);
}

void mkr_barrier_wait(mkr_barrier_t* barrier) {
    // Only num_threads are allowed to enter.
    sem_wait(&barrier->entrance);

    // Increase counter and check if this is the last thread to arrive.
    sem_wait(&barrier->counter_mutex);
    if (++barrier->counter == barrier->num_threads) {
        // If this is the last thread to arrive, unlock the gate for 1 thread.
        sem_post(&barrier->gate);
    }
    sem_post(&barrier->counter_mutex);

    // Wait at the gate.
    sem_wait(&barrier->gate);

    // Decrease counter and check if this is the last thread to cross the gate.
    sem_wait(&barrier->counter_mutex);
    if (--barrier->counter) {
        // If this is NOT the last thread to cross, unlock the gate for the next thread.
        sem_post(&barrier->gate);
    } else {
        // If this is the last thread to cross, allow the next batch of num_threads to enter.
        for (int i = 0; i < barrier->num_threads; ++i) {
            sem_post(&barrier->entrance);
        }
    }
    sem_post(&barrier->counter_mutex);
}

#endif // MKR_BARRIER_H