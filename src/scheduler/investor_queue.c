#include "investor_queue.h"

#include "log.h"

void init_queue(InvestorQueue *queue)
{
    queue->front = 0;
    queue->back = 0;
    queue->count = 0;
    pthread_mutex_init(&queue->lock, NULL);
}

void enqueue_investor(InvestorQueue *queue, Investor *investor)
{
    pthread_mutex_lock(&queue->lock);

    if (queue->count == MAX_INVESTORS)
    {
        log_message(LOG_WARNING, "Investor queue is full!\n");
    }
    else
    {
        queue->investors[queue->back] = investor;
        queue->back = (queue->back + 1) % MAX_INVESTORS;
        queue->count++;
    }

    pthread_mutex_unlock(&queue->lock);
}

Investor *dequeue_investor(InvestorQueue *queue)
{
    pthread_mutex_lock(&queue->lock);

    Investor *investor = NULL;
    if (queue->count == 0)
    {
        log_message(LOG_WARNING, "Investor queue is empty!\n");
    }
    else
    {
        investor = queue->investors[queue->front];
        queue->front = (queue->front + 1) % MAX_INVESTORS;
        queue->count--;
    }

    pthread_mutex_unlock(&queue->lock);

    return investor;
}
