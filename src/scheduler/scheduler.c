#include "scheduler.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "investor_queue.h"
#include "order.h"
#include "log.h"
#include "waitgroup.h"
#include "error_codes.h"
#include "book.h"

static const char *ASSETS[NUM_ASSETS] = {"PETR4", "VALE3", "ITUB4", "BBDC4", "ABEV3", "BBAS3", "WEGE3", "RENT3", "LREN3", "GGBR4"};

InvestorQueue queue;

void *investor_lifecycle(void *arg)
{
    Investor *investor = (Investor *)arg;
    srand(time(NULL) ^ (investor->id));

    for (int i = 0; i < MAX_OPERATIONS; i++)
    {
        int random_action = rand() % 2; // 0 = compra, 1 = venda
        int shares = (rand() % 100) + 1;

        char asset_code[6];
        int random_asset = rand() % NUM_ASSETS;
        snprintf(asset_code, 6, "%s", ASSETS[random_asset]);

        char asset_company[12];
        snprintf(asset_company, sizeof(asset_company), "%s S.A.", asset_code);

        Asset asset;
        create_asset(&asset, asset_code, asset_company, shares * shares);

        Order order;
        char *order_type_str;

        if (random_action == 0)
        {
            order_type_str = "BUY";
        }
        else
        {
            order_type_str = "SELL";

            Position new_position = {.shares = shares};
            strncpy(new_position.asset_code, asset_code, 6);
            add_asset_position(investor, &new_position);
        }

        // Adiciona no wait group antes de emitir a ordem
        add_waitgroup(book.wg, 1);
        int result = emit_order(&order, i, investor, &asset, shares, 100.0, order_type_str);
        if (result == SUCCESS)
        {
            enqueue_order(book.orders_channel_in, &order);
        }

        // Marca a operação como finalizada no wait group
        done_waitgroup(book.wg);

        sleep((rand() % 3) + 1);
    }

    pthread_exit(NULL);
}

void *scheduler_thread(void *arg)
{
    while (queue.count > 0)
    {
        Investor *investor = dequeue_investor(&queue);

        if (investor != NULL)
        {
            log_message(LOG_INFO, "Escalonando investidor: %s", investor->name);
            add_waitgroup(book.wg, 1);
            pthread_create(&investor->id, NULL, investor_lifecycle, (void *)investor);
        }

        sleep(INVESTOR_LIFETIME);
    }

    // Espera até que todas as threads de investidores terminem
    wait_waitgroup(book.wg);
    pthread_exit(NULL);
}

void simulate_investor_scheduling(Investor *investors, int num_investors)
{
    init_queue(&queue);

    for (int i = 0; i < num_investors; i++)
    {
        enqueue_investor(&queue, &investors[i]);
    }

    pthread_t scheduler;
    pthread_create(&scheduler, NULL, scheduler_thread, NULL);
    pthread_join(scheduler, NULL);

    destroy_waitgroup(book.wg);
}
