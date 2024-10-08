#include "transaction.h"

#include <stdio.h>

#include "log.h"
#include "error_codes.h"

int create_transaction(Transaction *transaction, Order *selling_order, Order *buying_order)
{
    if (selling_order == NULL || buying_order == NULL)
    {
        log_message(LOG_WARNING, "Selling or buying order cannot be NULL");
        return ERR_VALIDATION;
    }

    // O número deve ser o mínimo entre as ordens
    int shares = (selling_order->pending_shares < buying_order->pending_shares)
                     ? selling_order->pending_shares
                     : buying_order->pending_shares;

    // O preço da transação é o maior entre as ordens
    double price = (selling_order->price > buying_order->price)
                       ? selling_order->price
                       : buying_order->price;

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
    log_message(LOG_INFO, "Transaction created: '%s' Selling %d (shares = %d / pending = %d) of %s <-> '%s' Buying %d (shares = %d / pending = %d) of %s. Total R$ %.2f (shares = %d / price = R$ %.2f)",
                selling_order->investor->name, shares, selling_order->shares, selling_order->pending_shares, selling_order->asset->code,
                buying_order->investor->name, shares, buying_order->shares, buying_order->pending_shares, buying_order->asset->code,
                transaction->total, shares, price);

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
    if (transaction->buying_order->pending_shares + shares < 0)
    {
        log_message(LOG_WARNING, "Shares cannot be negative for buying order ID: %d", transaction->buying_order->id);
        return;
    }

    transaction->buying_order->pending_shares += shares;
}

void add_sell_order_pending_shares(Transaction *transaction, int shares)
{
    if (transaction->selling_order->pending_shares + shares < 0)
    {
        log_message(LOG_WARNING, "Shares cannot be negative for selling order ID: %d", transaction->selling_order->id);
        return;
    }

    transaction->selling_order->pending_shares += shares;
}
