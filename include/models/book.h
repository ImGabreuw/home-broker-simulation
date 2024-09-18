#ifndef BOOK_H
#define BOOK_H

#include <pthread.h>

#include "order_queue.h"
#include "order.h"
#include "transaction.h"

#define MAX_TRANSACTIONS 100

typedef struct {
    Order orders[MAX_ORDERS];
    Transaction transactions[MAX_TRANSACTIONS];
    OrderQueue *input_queue;
    OrderQueue *output_queue;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
} Book;

void init_book(Book *book, OrderQueue *input_queue, OrderQueue *output_queue, pthread_mutex_t *mutex, pthread_cond_t *cond);

void trade(Book *book);

void add_transaction(Book *book, Transaction *transaction);

#endif
