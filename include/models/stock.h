#ifndef STOCK_MODELS_H
#define STOCK_MODELS_H

#include <stdbool.h>

typedef struct Stock {
    char *code;
    char *companyName;
    double currentPrice;
    int availableAmount;
    bool isAvailable;
} stock;

// STOCK IPO
int create_stock(stock *stock, char *id, char *companyName, double initialValue, int avaliableAmount);

#endif // STOCK_MODELS_H
