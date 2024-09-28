#ifndef BOOK_H
#define BOOK_H

#include <pthread.h>
#include <semaphore.h>

#include "order.h"
#include "order_queue.h"
#include "transaction.h"
#include "waitgroup.h"

#define MAX_TRANSACTIONS 100

typedef struct
{
    Transaction transactions[MAX_TRANSACTIONS];
    OrderQueue *order_channel;
    pthread_mutex_t lock; // Mutex para proteger o acesso ao book
} Book;

extern Book book;

/**
 * @brief Inicializa o book de ordens.
 *
 * Esta função configura o book de ordens com os canais de entrada e saída de ordens
 * e um objeto de sincronização Waitgroup. Inicializa as filas de ordens de entrada
 * e saída.
 *
 * @param book Ponteiro para a estrutura Book a ser inicializada.
 * @param in Ponteiro para a fila de ordens de entrada.
 */
void init_book(Book *book, OrderQueue *order_channel);

/**
 * @brief Executa o processo de negociação no book de ordens.
 *
 * Esta função é responsável por processar ordens de compra e venda. Ela monitora
 * a fila de ordens de entrada, classifica as ordens em filas de compra e venda,
 * e tenta casar ordens opostas quando possível.
 *
 * @param book Ponteiro para a estrutura Book que contém as ordens e transações.
 */
void trade(Book *book);

/**
 * @brief Casa uma ordem de entrada com uma ordem oposta na fila.
 *
 * Esta função tenta casar uma ordem de compra ou venda recebida com uma ordem oposta
 * na fila correspondente (compra ou venda). Se uma correspondência for encontrada,
 * a transação é criada e as posições dos investidores são atualizadas.
 *
 * @param book Ponteiro para a estrutura Book que contém as ordens e transações.
 * @param incoming_order Ponteiro para a ordem de entrada a ser casada.
 * @param opposite_queue Ponteiro para a fila de ordens opostas (compra ou venda).
 */
int match_order(Book *book, Order *incoming_order, OrderQueue *opposite_queue);

/**
 * @brief Adiciona uma transação ao book.
 *
 * Esta função atualiza as posições dos investidores envolvidos na transação e
 * registra a transação no book de transações.
 *
 * @param book Ponteiro para a estrutura Book que contém as transações.
 * @param transaction Ponteiro para a transação a ser adicionada.
 */
void add_transaction(Book *book, Transaction *transaction);

/**
 * @brief Armazena uma transação no book de transações.
 *
 * Esta função busca um espaço disponível no book de transações e armazena
 * a transação fornecida. Se o book estiver cheio, um aviso será registrado.
 *
 * @param book Ponteiro para a estrutura Book que contém o book de transações.
 * @param transaction Ponteiro para a transação a ser armazenada.
 */
void store_transaction(Book *book, Transaction *transaction);

#endif // BOOK_H
