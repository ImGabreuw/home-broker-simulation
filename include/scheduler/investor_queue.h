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
    pthread_mutex_t lock; ///< Mutex para sincronização de acesso à fila.
} InvestorQueue;

/**
 * @brief Inicializa uma fila de investidores.
 *
 * Esta função inicializa uma estrutura `InvestorQueue`, configurando os índices, o contador e o mutex.
 * Deve ser chamada antes de qualquer outra operação com a fila.
 *
 * @param queue Ponteiro para a estrutura `InvestorQueue` que será inicializada.
 */
void init_queue(InvestorQueue *queue);

/**
 * @brief Adiciona um investidor à fila (enqueue).
 *
 * Insere um investidor na parte traseira da fila. A função é thread-safe e utiliza um mutex
 * para garantir que a inserção seja segura em ambientes concorrentes.
 *
 * @param queue Ponteiro para a estrutura `InvestorQueue` onde o investidor será adicionado.
 * @param investor Ponteiro para o investidor a ser enfileirado.
 *
 * @note Se a fila já estiver cheia, essa função não enfileira o investidor.
 */
void enqueue_investor(InvestorQueue *queue, Investor *investor);

/**
 * @brief Remove um investidor da fila (dequeue).
 *
 * Remove e retorna o investidor da frente da fila. A função é thread-safe e utiliza um mutex
 * para garantir que a remoção seja segura em ambientes concorrentes.
 *
 * @param queue Ponteiro para a estrutura `InvestorQueue` de onde o investidor será removido.
 *
 * @return Ponteiro para o investidor removido da fila, ou `NULL` se a fila estiver vazia.
 */
Investor *dequeue_investor(InvestorQueue *queue);

#endif // INVESTOR_QUEUE_H
