#include "scheduler.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "investor_queue.h"
#include "order.h"
#include "log.h"
#include "error_codes.h"
#include "book.h"
#include "asset.h"

InvestorQueue queue;

void *investor_lifecycle(void *arg)
{
    Investor *investor = (Investor *)arg;
    srand(time(NULL) ^ (investor->id));

    for (int i = 0; i < MAX_OPERATIONS; i++)
    {
        int random_action = rand() % 2; // 0 = compra, 1 = venda
        int shares = (rand() % 100) + 1;
        int random_asset = rand() % NUM_ASSETS;

        Order *order = (Order *)malloc(sizeof(Order));
        if (order == NULL)
        {
            log_message(LOG_ERROR, "Failed to allocate memory for new order.");
            pthread_exit(NULL);
        }

        char *order_type_str;

        if (random_action == 0)
        {
            order_type_str = "BUY";
        }
        else
        {
            order_type_str = "SELL";
        }

        int result = emit_order(order, i, investor, &assets[random_asset], shares, 100.0, order_type_str);
        if (result == SUCCESS)
        {
            enqueue_order(book.order_channel, order);
        }

        sleep((rand() % 6) + 1);
    }

    pthread_exit(NULL);
}

void *scheduler_thread(void *arg)
{
    while (queue.count > 0)
    {
        srand(time(NULL));
        // int pool = (rand() % 3) + 1;
        int pool = 1;
        for (int i = 0; i < pool; i++)
        {
            Investor *investor = dequeue_investor(&queue);

            if (investor != NULL)
            {
                log_message(LOG_INFO, "Escalonando investidor: %s", investor->name);
                pthread_create(&investor->id, NULL, investor_lifecycle, (void *)investor);
                enqueue_investor(&queue, investor);
            }
        }

        // Aguarda um tempo antes de escalonar o próximo investidor
        sleep(INVESTOR_LIFETIME);
    }

    // Espera até que todas as threads de investidores terminem
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
}
