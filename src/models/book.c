#include "book.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "error_codes.h"
#include "log.h"

void init_book(Book *book, OrderQueue *input_queue, OrderQueue *output_queue, pthread_mutex_t *mutex, pthread_cond_t *cond)
{
    book->input_queue = input_queue;
    book->output_queue = output_queue;
    book->mutex = mutex;
    book->cond = cond;

    memset(book->orders, 0, sizeof(book->orders));
    memset(book->transactions, 0, sizeof(book->transactions));
}

void trade(Book *book)
{
    OrderQueue buy_orders;
    init_order_queue(&buy_orders);

    OrderQueue sell_orders;
    init_order_queue(&sell_orders);

    while (1)
    {
        pthread_mutex_lock(book->mutex);
        while (size(book->input_queue) == 0)
        {
            pthread_cond_wait(book->cond, book->mutex);
        }

        Order order;
        int result = dequeue_order(book->input_queue, &order);
        pthread_mutex_unlock(book->mutex);

        if (result == ERR_VALIDATION)
        {
            continue;
        }

        char asset_code[MAX_ASSET_CODE_LENGTH];
        strcpy(asset_code, order.asset->code);

        if (order.action == BUY)
        {
            enqueue_order(&buy_orders, &order);

            Order sell_order;
            if (size(&sell_orders) != 0 && dequeue_order(&sell_orders, &sell_order) == SUCCESS)
            {
                if (sell_order.pending_shares > 0 && strcmp(sell_order.asset->code, asset_code) == 0)
                {
                    Transaction transaction;
                    create_transaction(&transaction, &sell_order, &order, order.shares, sell_order.price);
                    add_transaction(book, &transaction);

                    enqueue_order(book->output_queue, &sell_order);
                    enqueue_order(book->output_queue, &order);

                    if (sell_order.pending_shares > 0)
                    {
                        enqueue_order(&sell_orders, &sell_order);
                    }
                }
            }
        }
        else if (order.action == SELL)
        {
            enqueue_order(&sell_orders, &order);

            Order buy_order;
            if (size(&buy_orders) != 0 && dequeue_order(&buy_orders, &buy_order) == SUCCESS)
            {
                if (buy_order.pending_shares > 0 && strcmp(buy_order.asset->code, asset_code) == 0)
                {
                    Transaction transaction;
                    create_transaction(&transaction, &order, &buy_order, order.shares, buy_order.price);
                    add_transaction(book, &transaction);

                    enqueue_order(book->output_queue, &buy_order);
                    enqueue_order(book->output_queue, &order);

                    if (buy_order.pending_shares > 0)
                    {
                        enqueue_order(&buy_orders, &buy_order);
                    }
                }
            }
        }
    }
}

void add_transaction(Book *book, Transaction *transaction)
{
    int transaction_index = 0;
    while (transaction_index < MAX_TRANSACTIONS && book->transactions[transaction_index].buying_order != NULL)
    {
        transaction_index++;
    }

    if (transaction_index >= MAX_TRANSACTIONS)
    {
        return;
    }

    book->transactions[transaction_index] = *transaction;

    int shares_traded = transaction->shares;
    int buying_index = get_position_index(transaction->buying_order->investor, transaction->buying_order->asset->code);
    int selling_index = get_position_index(transaction->selling_order->investor, transaction->selling_order->asset->code);

    if (buying_index == ERR_NOT_FOUND && selling_index == ERR_NOT_FOUND)
    {
        log_message(LOG_WARNING, "Invalid asset code in positions array");
        return;
    }

    transaction->buying_order->investor->positions[buying_index].shares += shares_traded;
    transaction->selling_order->investor->positions[selling_index].shares -= shares_traded;

    if (transaction->buying_order->pending_shares == 0)
    {
        transaction->buying_order->status = CLOSE;
    }

    if (transaction->selling_order->pending_shares == 0)
    {
        transaction->selling_order->status = CLOSE;
    }

    log_message(LOG_INFO, "Transaction processed: %d shares traded for asset code %d", shares_traded, transaction->buying_order->asset->code);
}
