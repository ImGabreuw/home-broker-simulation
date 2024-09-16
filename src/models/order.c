#include "order.h"

#include <string.h>
#include <stdio.h>

#include "log.h"

int emit_order(Order *order, int id, Investor *investor, Asset *asset, int shares, double price, const char *order_type_str)
{
    if (investor == NULL || asset == NULL)
    {
        log_message(LOG_WARNING, "Investor or Asset cannot be NULL\n");
        return ERR_VALIDATION;
    }

    if (shares <= 0 || price <= 0.0)
    {
        log_message(LOG_WARNING, "Shares and price must be greater than zero\n");
        return ERR_VALIDATION;
    }

    if (strcmp(order_type_str, "BUY") == 0)
    {
        order->action = BUY;
    }
    else if (strcmp(order_type_str, "SELL") == 0)
    {
        order->action = SELL;
    }
    else
    {
        log_message(LOG_WARNING, "Invalid order type\n");
        return ERR_VALIDATION;
    }

    order->id = id;
    order->investor = investor;
    order->asset = asset;
    order->shares = shares;
    order->pending_shares = shares;
    order->price = price;
    order->status = OPEN;
    order->transactions = NULL;

    log_message(LOG_INFO, "Order created: ID: %d, Type: %s, Shares: %d, Price: %.2f\n",
                id, order_type_str, shares, price);
    return SUCCESS;
}
