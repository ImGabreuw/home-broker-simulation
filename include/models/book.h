#ifndef BOOK_H
#define BOOK_H

#include "order.h"
#include "order_queue.h"
#include "transaction.h"
#include "waitgroup.h"

#define MAX_TRANSACTIONS 100

typedef struct {
    Order orders[MAX_ORDERS];
    Transaction transactions[MAX_TRANSACTIONS];
    OrderQueue* orders_channel_in;
    OrderQueue* orders_channel_out;
    Waitgroup* wg;
} Book;

void init_book(Book* book, OrderQueue* in, OrderQueue* out, Waitgroup* wg);

void trade(Book* book);

void add_transaction(Book* book, Transaction* transaction);

#endif // BOOK_H
