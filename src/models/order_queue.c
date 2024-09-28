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

    // Inicializa semáforos
    sem_init(&queue->sem_empty, 0, MAX_ORDERS); // Inicializa como MAX_ORDERS
    sem_init(&queue->sem_full, 0, 0);           // Inicializa como 0
    pthread_mutex_init(&queue->mutex, NULL);    // Inicializa o mutex

    return SUCCESS;
}

int enqueue_order(OrderQueue *queue, Order *order)
{
    if (queue == NULL || order == NULL)
    {
        log_message(LOG_WARNING, "Invalid parameters: queue or order is NULL");
        return ERR_VALIDATION;
    }

    // Espera até que haja espaço na fila
    sem_wait(&queue->sem_empty);

    // Protege o acesso à fila com mutex
    pthread_mutex_lock(&queue->mutex);

    queue->back = (queue->back + 1) % MAX_ORDERS;
    queue->orders[queue->back] = *order;
    queue->count++;

    // Libera o semáforo de ordens
    sem_post(&queue->sem_full);

    pthread_mutex_unlock(&queue->mutex); // Desbloqueia o mutex

    return SUCCESS;
}

int dequeue_order(OrderQueue *queue, Order *order)
{
    if (queue == NULL || order == NULL)
    {
        log_message(LOG_WARNING, "Invalid parameters: queue or order is NULL");
        return ERR_VALIDATION;
    }

    // Espera até que haja ordens na fila
    sem_wait(&queue->sem_full);

    // Protege o acesso à fila com mutex
    pthread_mutex_lock(&queue->mutex);

    *order = queue->orders[queue->front];
    queue->front = (queue->front + 1) % MAX_ORDERS;
    queue->count--;

    char action_name[5];
    get_action_name(order, action_name);

    // Libera o semáforo de espaço vazio
    sem_post(&queue->sem_empty);

    pthread_mutex_unlock(&queue->mutex);

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
