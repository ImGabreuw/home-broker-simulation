#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include <semaphore.h>
#include <pthread.h>

#include "order.h"

#define MAX_ORDERS 100

typedef struct
{
    Order orders[MAX_ORDERS];
    int front;
    int back;
    int count;
    sem_t sem_empty;       ///< Semáforo que controla a quantidade de espaços vazios na fila.
    sem_t sem_full;        ///< Semáforo que controla a quantidade de ordens na fila.
    pthread_mutex_t mutex; ///< Mutex para garantir acesso exclusivo à fila.
} OrderQueue;

/**
 * @brief Inicializa a fila de ordens (OrderQueue).
 *
 * Esta função configura a fila, inicializando os semáforos e o mutex. A fila é configurada para começar vazia.
 *
 * @param queue Ponteiro para a estrutura `OrderQueue` que será inicializada.
 *
 * @return Retorna 0 (SUCCESS) em caso de sucesso ou um código de erro caso ocorra uma falha na inicialização.
 *
 * @note Esta função deve ser chamada antes de utilizar a fila.
 */
int init_order_queue(OrderQueue *queue);

/**
 * @brief Enfileira uma nova ordem na fila.
 *
 * Adiciona uma ordem ao final da fila de ordens. A função usa semáforos para garantir que não ultrapasse o limite da fila
 * e mutexes para sincronizar o acesso à fila entre múltiplas threads.
 *
 * @param queue Ponteiro para a estrutura `OrderQueue`.
 * @param order Ponteiro para a estrutura `Order` que será enfileirada.
 *
 * @return Retorna 0 (SUCCESS) em caso de sucesso, ou um código de erro caso a operação falhe.
 *
 * @note A função irá bloquear caso a fila esteja cheia até que haja espaço disponível.
 */
int enqueue_order(OrderQueue *queue, Order *order);

/**
 * @brief Remove uma ordem da fila.
 *
 * Remove a ordem da frente da fila e a coloca na variável `order`. A função utiliza semáforos para garantir que há ordens
 * na fila antes de tentar removê-las e mutexes para proteger o acesso.
 *
 * @param queue Ponteiro para a estrutura `OrderQueue`.
 * @param order Ponteiro para a estrutura `Order` onde a ordem removida será armazenada.
 *
 * @return Retorna 0 (SUCCESS) em caso de sucesso, ou um código de erro caso não haja ordens disponíveis ou a operação falhe.
 *
 * @note A função irá bloquear caso a fila esteja vazia até que haja uma nova ordem para ser removida.
 */
int dequeue_order(OrderQueue *queue, Order *order);

/**
 * @brief Retorna o número de ordens atualmente presentes na fila.
 * 
 * Esta função simplesmente retorna o valor do campo `count`, que representa o número de ordens atualmente na fila.
 * A função é protegida por um mutex para garantir a integridade dos dados em ambientes multi-thread.
 * 
 * @param queue Ponteiro para a estrutura `OrderQueue`.
 * 
 * @return Retorna o número de ordens presentes na fila.
 */
int size(OrderQueue *queue);

#endif
