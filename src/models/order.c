#include "order.h"

#include <string.h>
#include <stdio.h>

#include "log.h"
#include "error_codes.h"

int emit_order(Order *order, int id, Investor *investor, Asset *asset, int shares, double price, const char *order_type_str)
{
    if (investor == NULL || asset == NULL)
    {
        log_message(LOG_WARNING, "Investor or Asset cannot be NULL");
        return ERR_VALIDATION;
    }

    if (shares <= 0 || price <= 0.0)
    {
        log_message(LOG_WARNING, "Shares and price must be greater than zero");
        return ERR_VALIDATION;
    }

    if (strcmp(order_type_str, "BUY") == 0)
    {
        order->action = BUY;
    }
    else if (strcmp(order_type_str, "SELL") == 0)
    {
        order->action = SELL;
        Position *position = get_asset_position(investor, asset->code);

        if (position == NULL)
        {
            log_message(LOG_WARNING, "Cannot emit sell order for asset %s that is not in investor position.", asset->code);
            return ERR_VALIDATION;
        }

        if (shares > position->shares)
        {
            log_message(LOG_WARNING, "Sell order cannot be emitted. Investor does not have enough shares: Requested: %d, Available: %d",
                        shares, position->shares);
            return ERR_VALIDATION;
        }
    }
    else
    {
        log_message(LOG_WARNING, "Invalid order type");
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

    log_message(LOG_INFO, "%s order emitted: ID: %d, Asset: %s, Shares: %d, Price: %.2f",
                order_type_str, id, asset->code, shares, price);
    return SUCCESS;
}

void get_action_name(Order *order, char *action_name)
{
    if (order->action == 0)
    {
        strcpy(action_name, "BUY");
    }
    else
    {
        strcpy(action_name, "SELL");
    }
}
