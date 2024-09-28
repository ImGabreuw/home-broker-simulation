#include "book.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error_codes.h"
#include "log.h"
#include "asset.h"

void init_book(Book *book, OrderQueue *order_channel)
{
    book->order_channel = order_channel;

    init_order_queue(order_channel);
    pthread_mutex_init(&book->lock, NULL);

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

        // Protege o acesso ao book com mutex
        pthread_mutex_lock(&book->lock);

        // Pega a nova ordem do canal de entrada
        if (dequeue_order(book->order_channel, &order) == SUCCESS)
        {
            char *asset_code = order.asset->code;
            char action_name[5];
            get_action_name(&order, action_name);
            log_message(LOG_INFO, "Processing %s order: %s %d shares at price %.2f", action_name, asset_code, order.shares, order.price);

            // Adiciona a nova ordem à fila correspondente (compra ou venda)
            if (order.action == BUY)
            {
                enqueue_order(&buy_orders, &order);
                log_message(LOG_INFO, "Buy order added to 'buy_orders' for %s", order.asset->code);
            }
            else if (order.action == SELL)
            {
                enqueue_order(&sell_orders, &order);
                log_message(LOG_INFO, "Sell order added to 'sell_orders' for %s", order.asset->code);
            }
        }

        pthread_mutex_unlock(&book->lock); // Libera o acesso ao book

        // Tenta casar as ordens
        log_message(LOG_INFO, "Buy Orders: %d / Sell Oders: %d", buy_orders.count, sell_orders.count);
        if (buy_orders.count > 0 && sell_orders.count > 0)
        {
            Order incoming_order;
            if (buy_orders.count > 0)
            {
                dequeue_order(&buy_orders, &incoming_order);
                int matched = match_order(book, &incoming_order, &sell_orders);

                if (!matched)
                {
                    log_message(LOG_INFO, "No matching order found for incoming order: %s", incoming_order.asset->code);
                    enqueue_order(&buy_orders, &incoming_order);
                }
            }
            else if (sell_orders.count > 0)
            {
                dequeue_order(&sell_orders, &incoming_order);
                int matched = match_order(book, &incoming_order, &buy_orders);

                if (!matched)
                {
                    log_message(LOG_INFO, "No matching order found for incoming order: %s", incoming_order.asset->code);
                    enqueue_order(&sell_orders, &incoming_order);
                }
            }
        }
        else
        {
            log_message(LOG_INFO, "No matching orders to process. Waiting for new orders...");
        }

        sleep(1);
        log_message(LOG_INFO, "Order processing completed.");
    }
}

int match_order(Book *book, Order *incoming_order, OrderQueue *opposite_queue)
{
    Order matched_order;
    int matched = 0;
    int attempts = opposite_queue->count;

    // Tenta casar a ordem atual com ordens opostas
    while (opposite_queue->count > 0 && attempts > 0)
    {
        attempts--;
        if (dequeue_order(opposite_queue, &matched_order) != SUCCESS)
        {
            log_message(LOG_WARNING, "No matched order available in the opposite queue.");
            return -1;
        }

        // Verifica se os códigos dos ativos são iguais
        if (strcmp(incoming_order->asset->code, matched_order.asset->code) == 0)
        {
            log_message(LOG_INFO, "Matching order found: %s", incoming_order->asset->code);
            matched = 1; // Indica que encontramos um match

            // Verifica se a ordem combinada ainda tem ações pendentes
            if (matched_order.pending_shares > 0)
            {
                Transaction transaction;
                int result = create_transaction(&transaction, incoming_order, &matched_order);

                if (result != SUCCESS)
                {
                    log_message(LOG_WARNING, "Transaction creation failed.");
                    return -1;
                }

                add_transaction(book, &transaction);

                if (matched_order.status == CANCELED)
                {
                    log_message(LOG_WARNING, "Matched order was canceled. Transaction will not be added.");
                    return -1;
                }

                // Envia as ordens correspondentes ao canal de saída
                enqueue_order(book->order_channel, &matched_order);
                enqueue_order(book->order_channel, incoming_order);

                // Se ainda houver ações pendentes, re-queue a ordem combinada
                if (matched_order.pending_shares > 0)
                {
                    enqueue_order(opposite_queue, &matched_order);
                    log_message(LOG_INFO, "Re-queued remaining shares for matched order.");
                }
            }
            break; // Sai do loop uma vez que encontramos um match
        }
        else
        {
            // Se a ordem não combinar, enfileira de volta
            enqueue_order(opposite_queue, &matched_order);
        }
    }

    return matched;
}

void add_transaction(Book *book, Transaction *transaction)
{
    Investor *selling_investor = transaction->selling_order->investor;
    Investor *buying_investor = transaction->buying_order->investor;

    int shares_transacted = transaction->shares;

    // Obtendo as posições dos investidores para o ativo transacionado
    Position *selling_position = get_asset_position(selling_investor, transaction->selling_order->asset->code);
    Position *buying_position = get_asset_position(buying_investor, transaction->buying_order->asset->code);

    // Verificar se o vendedor tem ações suficientes
    if (selling_position == NULL || selling_position->shares < shares_transacted)
    {
        log_message(LOG_WARNING, "Transaction canceled: seller does not have enough shares.");
        transaction->selling_order->status = CANCELED;
        return;
    }

    // Atualiza a posição do vendedor
    update_asset_position(selling_investor, transaction->selling_order->asset->code, selling_position->shares - shares_transacted);

    // Atualiza a posição do comprador
    if (buying_position == NULL)
    {
        // Inicializa a posição se o comprador não tiver ações do ativo
        buying_position = (Position *)malloc(sizeof(Position));
        buying_position->shares = 0;
        strncpy(buying_position->asset_code, transaction->buying_order->asset->code, MAX_ASSET_CODE_LENGTH);
        buying_position->asset_code[MAX_ASSET_CODE_LENGTH - 1] = '\0';
    }

    // Atualiza a posição do comprador
    update_asset_position(buying_investor, transaction->buying_order->asset->code, buying_position->shares + shares_transacted);

    // Atualiza o número de shares pendentes das ordens
    transaction->selling_order->pending_shares -= shares_transacted;
    transaction->buying_order->pending_shares -= shares_transacted;

    // Se a ordem de venda estiver totalmente preenchida, atualize seu status
    if (transaction->selling_order->pending_shares == 0)
    {
        close_sell_order(transaction);
    }

    // Se a ordem de compra estiver totalmente preenchida, atualize seu status
    if (transaction->buying_order->pending_shares == 0)
    {
        close_buy_order(transaction);
    }

    store_transaction(book, transaction);

    calculate_total(transaction);
    log_message(LOG_INFO, "Transaction executed and positions updated.");
}

void store_transaction(Book *book, Transaction *transaction)
{
    for (int i = 0; i < MAX_TRANSACTIONS; i++)
    {
        if (book->transactions[i].buying_order == NULL)
        {
            book->transactions[i] = *transaction;
            log_message(LOG_INFO, "Transaction stored at position %d in the book.", i + 1);
            break;
        }
    }
}
