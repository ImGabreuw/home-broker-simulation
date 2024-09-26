#include "book.h"
#include <stdlib.h>
#include <string.h>
#include "error_codes.h"
#include "log.h"

void init_book(Book *book, OrderQueue *in, OrderQueue *out, Waitgroup *wg)
{
    book->orders_channel_in = in;
    book->orders_channel_out = out;
    book->wg = wg;

    memset(book->orders, 0, sizeof(book->orders));
    memset(book->transactions, 0, sizeof(book->transactions));

    log_message(LOG_INFO, "Initialized book with input and output order channels.");
}

void trade(Book *book)
{
    OrderQueue buy_orders;
    init_order_queue(&buy_orders);
    log_message(LOG_INFO, "Initialized buy orders queue.");

    OrderQueue sell_orders;
    init_order_queue(&sell_orders);
    log_message(LOG_INFO, "Initialized sell orders queue.");

    while (1)
    {
        Order order;
        if (dequeue_order(book->orders_channel_in, &order) != 0)
        {
            log_message(LOG_WARNING, "No order received. Continuing to next iteration.");
            continue;
        }

        char *asset_code = order.asset->code;
        log_message(LOG_INFO, "Processing order: %s %s %d shares at price %.2f",
                    order.action == BUY ? "BUY" : "SELL",
                    asset_code,
                    order.shares,
                    order.price);

        if (order.action == BUY)
        {
            enqueue_order(&buy_orders, &order);
            log_message(LOG_INFO, "Enqueued buy order.");

            // Verifica se existem ordens de venda para combinar
            if (sell_orders.count > 0 && sell_orders.orders[0].price <= order.price)
            {
                Order sell_order;
                dequeue_order(&sell_orders, &sell_order);

                if (sell_order.pending_shares > 0)
                {
                    Transaction transaction;
                    int result = create_transaction(&transaction, &sell_order, &order, order.shares, sell_order.price);

                    if (result == ERR_VALIDATION)
                    {
                        log_message(LOG_WARNING, "Failed to create transaction for buy order.");
                        continue;
                    }

                    add_transaction(book, &transaction);
                    log_message(LOG_INFO, "Transaction created: %s sold to %s",
                                sell_order.asset->code,
                                order.asset->code);

                    enqueue_order(book->orders_channel_out, &sell_order);
                    enqueue_order(book->orders_channel_out, &order);

                    if (sell_order.pending_shares > 0)
                    {
                        enqueue_order(&sell_orders, &sell_order);
                        log_message(LOG_INFO, "Sell order re-queued.");
                    }
                }
            }
        }

        if (order.action == SELL)
        {
            enqueue_order(&sell_orders, &order);
            log_message(LOG_INFO, "Enqueued sell order.");

            // Verifica se existem ordens de compra para combinar
            if (buy_orders.count > 0 && buy_orders.orders[0].price >= order.price)
            {
                Order buy_order;
                dequeue_order(&buy_orders, &buy_order);

                if (buy_order.pending_shares > 0)
                {
                    Transaction transaction;
                    int result = create_transaction(&transaction, &order, &buy_order, order.shares, buy_order.price);

                    if (result == ERR_VALIDATION)
                    {
                        log_message(LOG_WARNING, "Failed to create transaction for sell order.");
                        continue;
                    }

                    add_transaction(book, &transaction);
                    log_message(LOG_INFO, "Transaction created: %s bought from %s",
                                buy_order.asset->code,
                                order.asset->code);

                    enqueue_order(book->orders_channel_out, &buy_order);
                    enqueue_order(book->orders_channel_out, &order);

                    if (buy_order.pending_shares > 0)
                    {
                        enqueue_order(&buy_orders, &buy_order);
                        log_message(LOG_INFO, "Buy order re-queued.");
                    }
                }
            }
        }

        done_waitgroup(book->wg);
        log_message(LOG_INFO, "Order processed and Wait Group updated.");
    }
}

void add_transaction(Book *book, Transaction *transaction)
{
    int selling_shares = transaction->selling_order->pending_shares;
    int buying_shares = transaction->buying_order->pending_shares;

    // Determina a quantidade mínima de ações que serão transacionadas
    int min_shares = selling_shares < buying_shares ? selling_shares : buying_shares;

    // Atualiza as posições dos investidores apenas se houver ações para transacionar
    if (min_shares > 0)
    {
        update_asset_position(transaction->selling_order->investor, transaction->selling_order->asset->code, -min_shares);
        add_buy_order_pending_shares(transaction, -min_shares);
        update_asset_position(transaction->buying_order->investor, transaction->buying_order->asset->code, min_shares);
        add_sell_order_pending_shares(transaction, -min_shares);
    }
    else
    {
        log_message(LOG_WARNING, "No shares to process for transaction, skipping...");
    }

    calculate_total(transaction);
    log_message(LOG_INFO, "Total calculated for transaction.");

    close_buy_order(transaction);
    close_sell_order(transaction);
    log_message(LOG_INFO, "Buy and sell orders closed.");

    int transaction_index = 0;
    while (transaction_index < MAX_TRANSACTIONS && book->transactions[transaction_index].buying_order != NULL)
    {
        transaction_index++;
    }

    if (transaction_index < MAX_TRANSACTIONS)
    {
        book->transactions[transaction_index] = *transaction;
        log_message(LOG_INFO, "Transaction stored in book at index %d.", transaction_index);
    }
    else
    {
        log_message(LOG_WARNING, "Transaction limit reached, cannot add more transactions.");
    }
}
