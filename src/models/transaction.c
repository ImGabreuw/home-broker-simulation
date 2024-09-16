#include "transaction.h"

#include <stdio.h>

#include "log.h"
#include "error_codes.h"

int create_transaction(Transaction *transaction, Order *selling_order, Order *buying_order, int shares, double price)
{
    if (selling_order == NULL || buying_order == NULL)
    {
        log_message(LOG_WARNING, "Selling or buying order cannot be NULL");
        return ERR_VALIDATION;
    }

    if (shares <= 0 || price <= 0.0)
    {
        log_message(LOG_WARNING, "Shares and price must be greater than zero");
        return ERR_VALIDATION;
    }

    transaction->selling_order = selling_order;
    transaction->buying_order = buying_order;
    transaction->shares = shares;
    transaction->price = price;
    transaction->created_at = time(NULL);

    calculate_total(transaction);
    log_message(LOG_INFO, "Transaction created: Selling Order ID: %d, Buying Order ID: %d, Shares: %d, Price: %.2f, Total: %.2f",
                selling_order->id, buying_order->id, shares, price, transaction->total);

    return SUCCESS;
}

void calculate_total(Transaction *transaction)
{
    transaction->total = transaction->shares * transaction->price;
}

void close_buy_order(Transaction *transaction)
{
    transaction->buying_order->status = CLOSE;
    log_message(LOG_INFO, "Buy order closed: Order ID: %d", transaction->buying_order->id);
}

void close_sell_order(Transaction *transaction)
{
    transaction->selling_order->status = CLOSE;
    log_message(LOG_INFO, "Sell order closed: Order ID: %d", transaction->selling_order->id);
}

void add_buy_order_pending_shares(Transaction *transaction, int shares)
{
    transaction->buying_order->pending_shares += shares;
    log_message(LOG_INFO, "Added %d shares to pending shares for buying order ID: %d", shares, transaction->buying_order->id);
}

void add_sell_order_pending_shares(Transaction *transaction, int shares)
{
    transaction->selling_order->pending_shares += shares;
    log_message(LOG_INFO, "Added %d shares to pending shares for selling order ID: %d", shares, transaction->selling_order->id);
}
