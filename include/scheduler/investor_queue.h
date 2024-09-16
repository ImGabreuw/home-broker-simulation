#ifndef INVESTOR_QUEUE_H
#define INVESTOR_QUEUE_H

#include <pthread.h>

#include "investor.h"

#define MAX_INVESTORS 10

typedef struct
{
    Investor *investors[MAX_INVESTORS];
    int front;
    int back;
    int count;
    pthread_mutex_t lock;
} InvestorQueue;

void init_queue(InvestorQueue *queue);

void enqueue_investor(InvestorQueue *queue, Investor *investor);

Investor *dequeue_investor(InvestorQueue *queue);

#endif // INVESTOR_QUEUE_H
