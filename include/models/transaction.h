#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "order.h"

#include <time.h>

// Fix circular typedef dependency (https://stackoverflow.com/questions/888386/resolve-circular-typedef-dependency)
typedef struct Transaction Transaction;
typedef struct Order Order;

struct Transaction
{
    int id;
    Order *selling_order;
    Order *buying_order;
    int shares;
    double price;
    double total;
    time_t created_at;
};

int create_transaction(Transaction *transaction, Order *selling_order, Order *buying_order, int shares, double price);

void calculate_total(Transaction *transaction);

void close_buy_order(Transaction *transaction);

void close_sell_order(Transaction *transaction);

void add_buy_order_pending_shares(Transaction *transaction, int shares);

void add_sell_order_pending_shares(Transaction *transaction, int shares);

#endif // TRANSACTION_H
