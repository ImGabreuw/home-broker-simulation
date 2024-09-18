#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include "order.h"

#define MAX_ORDERS 100

typedef struct
{
    Order orders[MAX_ORDERS];
    int front;
    int back;
    int count;
} OrderQueue;

int init_order_queue(OrderQueue *queue);

int enqueue_order(OrderQueue *queue, Order *order);

int dequeue_order(OrderQueue *queue, Order *order);

int size(OrderQueue *queue);

#endif
