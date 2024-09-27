#ifndef ORDER_H
#define ORDER_H

#include "investor.h"
#include "asset.h"
#include "transaction.h"

typedef enum
{
    BUY,
    SELL
} OrderAction;

typedef enum
{
    OPEN,
    CLOSE
} OrderStatus;

// Fix circular typedef dependency (https://stackoverflow.com/questions/888386/resolve-circular-typedef-dependency)
typedef struct Transaction Transaction;
typedef struct Order Order;

struct Order
{
    int id;
    Investor *investor;
    Asset *asset;
    int shares;
    int pending_shares;
    double price;
    OrderAction action;
    OrderStatus status;
    Transaction *transactions;
};

int emit_order(Order *order, int id, Investor *investor, Asset *asset, int shares, double price, const char *order_type_str);

void get_action_name(Order *order, char *action_name);

#endif // ORDER_H
