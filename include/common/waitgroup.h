#ifndef WAITGROUP_H
#define WAITGROUP_H

#include <pthread.h>

typedef struct
{
    int waitGroupSize;
    pthread_mutex_t mutex;
    pthread_cond_t cond; 
} Waitgroup;

int init_waitgroup(Waitgroup *wg);

void add_waitgroup(Waitgroup *wg, int size);

void done_waitgroup(Waitgroup *wg);

void wait_waitgroup(Waitgroup *wg);

void destroy_waitgroup(Waitgroup *wg); 

#endif // WAITGROUP_H
