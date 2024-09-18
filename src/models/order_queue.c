#include "order_queue.h"

#include <stdio.h>
#include <string.h>

#include "log.h"
#include "error_codes.h"

int init_order_queue(OrderQueue *queue)
{
    if (queue == NULL)
    {
        log_message(LOG_ERROR, "Queue is NULL");
        return ERR_VALIDATION;
    }

    queue->front = 0;
    queue->back = -1;
    queue->count = 0;

    log_message(LOG_INFO, "Queue initialized");
    return SUCCESS;
}

int enqueue_order(OrderQueue *queue, Order *order)
{
    if (queue == NULL || order == NULL)
    {
        log_message(LOG_WARNING, "Invalid parameters: queue or order is NULL");
        return ERR_VALIDATION;
    }

    if (queue->count >= MAX_ORDERS)
    {
        log_message(LOG_WARNING, "Queue is full");
        return ERR_VALIDATION;
    }

    queue->back = (queue->back + 1) % MAX_ORDERS;
    queue->orders[queue->back] = *order;
    queue->count++;

    log_message(LOG_INFO, "Order enqueued: Asset Code = %s", order->asset->code);
    return SUCCESS;
}

int dequeue_order(OrderQueue *queue, Order *order)
{
    if (queue == NULL || order == NULL)
    {
        log_message(LOG_WARNING, "Invalid parameters: queue or order is NULL");
        return ERR_VALIDATION;
    }

    if (queue->count == 0)
    {
        log_message(LOG_WARNING, "Queue is empty");
        return ERR_VALIDATION;
    }

    *order = queue->orders[queue->front];
    queue->front = (queue->front + 1) % MAX_ORDERS;
    queue->count--;

    log_message(LOG_INFO, "Order dequeued: Asset Code = %s", order->asset->code);
    return SUCCESS;
}

int size(OrderQueue *queue)
{
    if (queue == NULL)
    {
        log_message(LOG_WARNING, "Queue is NULL");
        return ERR_VALIDATION;
    }

    return queue->count;
}
