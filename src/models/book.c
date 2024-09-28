#include "book.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error_codes.h"
#include "log.h"
#include "asset.h"

void init_book(Book *book, OrderQueue *in, OrderQueue *out, Waitgroup *wg)
{
    book->orders_channel_in = in;
    book->orders_channel_out = out;
    book->wg = wg;

    init_order_queue(in);
    init_order_queue(out);

    log_message(LOG_INFO, "Initialized book with input and output order channels.");
}

void trade(Book *book)
{
    OrderQueue buy_orders;
    OrderQueue sell_orders;

    init_order_queue(&buy_orders);
    init_order_queue(&sell_orders);

    log_message(LOG_INFO, "Buy and sell order queues initialized.");

    while (1)
    {
        Order order;
        if (dequeue_order(book->orders_channel_in, &order) != 0)
        {
            sleep(2); // Simulating delay to listen to order channel
            log_message(LOG_WARNING, "No order received. Continuing to next iteration.");
            continue;
        }

        char *asset_code = order.asset->code;
        char action_name[5];
        get_action_name(&order, action_name);
        log_message(LOG_INFO, "Processing order: %s %s %d shares at price %.2f", action_name, asset_code, order.shares, order.price);

        if (order.action == BUY)
        {
            enqueue_order(&buy_orders, &order);
            log_message(LOG_INFO, "Buy order added to queue: %s", order.asset->code);

            if (sell_orders.count > 0 && sell_orders.orders[0].price <= order.price)
            {
                match_order(book, &order, &sell_orders);
            }
        }
        else if (order.action == SELL)
        {
            enqueue_order(&sell_orders, &order);
            log_message(LOG_INFO, "Sell order added to queue: %s", order.asset->code);

            if (buy_orders.count > 0 && buy_orders.orders[0].price >= order.price)
            {
                match_order(book, &order, &buy_orders);
            }
        }

        done_waitgroup(book->wg);
        log_message(LOG_INFO, "Order processed, wait group updated.");
    }
}

void match_order(Book *book, Order *incoming_order, OrderQueue *opposite_queue)
{
    Order matched_order;
    dequeue_order(opposite_queue, &matched_order);

    if (matched_order.pending_shares > 0)
    {
        Transaction transaction;
        int result = create_transaction(&transaction, incoming_order, &matched_order, incoming_order->shares, matched_order.price);

        if (result != SUCCESS)
        {
            log_message(LOG_WARNING, "Transaction creation failed.");
            return;
        }

        if (matched_order.status == CANCELED)
        {
            log_message(LOG_WARNING, "Matched order was canceled. Transaction will not be added.");
            return;
        }

        add_transaction(book, &transaction);

        enqueue_order(book->orders_channel_out, &matched_order);
        enqueue_order(book->orders_channel_out, incoming_order);

        if (matched_order.pending_shares > 0)
        {
            enqueue_order(opposite_queue, &matched_order);
            log_message(LOG_INFO, "Re-queued remaining shares for matched order.");
        }
    }
}

void add_transaction(Book *book, Transaction *transaction)
{
    Investor *selling_investor = transaction->selling_order->investor;
    Investor *buying_investor = transaction->buying_order->investor;

    int min_shares = transaction->shares;

    Position *selling_position = get_asset_position(selling_investor, transaction->selling_order->asset->code);
    Position *buying_position = get_asset_position(buying_investor, transaction->buying_order->asset->code);

    if (selling_position == NULL || selling_position->shares < min_shares)
    {
        log_message(LOG_WARNING, "Transaction canceled: seller does not have enough shares.");
        transaction->selling_order->status = CANCELED;
        return;
    }

    update_asset_position(selling_investor, transaction->selling_order->asset->code, selling_position->shares - min_shares);

    if (buying_position == NULL)
    {
        buying_position = (Position *)malloc(sizeof(Position));
        buying_position->shares = 0;
        strncpy(buying_position->asset_code, transaction->buying_order->asset->code, MAX_ASSET_CODE_LENGTH);
        buying_position->asset_code[MAX_ASSET_CODE_LENGTH - 1] = '\0';
    }

    update_asset_position(buying_investor, transaction->buying_order->asset->code, buying_position->shares + min_shares);

    calculate_total(transaction);
    log_message(LOG_INFO, "Transaction executed and positions updated.");

    store_transaction(book, transaction);
}

void store_transaction(Book *book, Transaction *transaction)
{
    for (int i = 0; i < MAX_TRANSACTIONS; i++)
    {
        if (book->transactions[i].buying_order == NULL)
        {
            book->transactions[i] = *transaction;
            log_message(LOG_INFO, "Transaction stored at index %d.", i);
            return;
        }
    }

    log_message(LOG_WARNING, "Transaction book full, cannot store more transactions.");
}
