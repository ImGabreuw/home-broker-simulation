#ifndef INVESTOR_MODELS_H
#define INVESTOR_MODELS_H

#include <pthread.h>
#include "stock.h"

#define MAX_STOCKS 10

typedef struct Investor {
    pthread_t id;
    char *name;
    double currentBalance;
    stock ownedStocks[MAX_STOCKS];
} investor;

int create_investor(investor *investor, pthread_t id, char *name, double currentBalance);

int buy_stock(investor *investor, stock *stock);

int sell_stock(investor *investor, stock *stock);

#endif // INVESTOR_MODELS_H
