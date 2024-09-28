#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include <semaphore.h> // Include necessário para semáforos
#include <pthread.h>   // Include necessário para mutexes

#include "order.h"

#define MAX_ORDERS 100

typedef struct
{
    Order orders[MAX_ORDERS];
    int front;
    int back;
    int count;
    sem_t sem_empty;       // Semáforo para controlar a quantidade de espaço vazio na fila
    sem_t sem_full;        // Semáforo para controlar a quantidade de ordens na fila
    pthread_mutex_t mutex; // Mutex para proteção de dados
} OrderQueue;

int init_order_queue(OrderQueue *queue);

int enqueue_order(OrderQueue *queue, Order *order);

int dequeue_order(OrderQueue *queue, Order *order);

int size(OrderQueue *queue);

#endif
