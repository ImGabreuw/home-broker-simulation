#include "scheduler.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "investor_queue.h"
#include "log.h"

InvestorQueue queue;

void *investor_lifecycle(void *arg)
{
    Investor *investor = (Investor *)arg;
    srand(time(NULL) ^ (investor->id)); // Geração de operações aleatórias por investidor

    for (int i = 0; i < MAX_OPERATIONS; i++)
    {
        int random_action = rand() % 2; // 0 = compra, 1 = venda
        int shares = (rand() % 100) + 1;
        char asset_code[6];
        snprintf(asset_code, 6, "A%d", rand() % 1000); // Geração de código de ativo aleatório

        if (random_action == 0)
        {
            Position new_position = {.shares = shares};
            strncpy(new_position.asset_code, asset_code, 6);
            add_asset_position(investor, &new_position);
            log_message(LOG_INFO, "Investor %s buys %d shares of stock %s\n", investor->name, shares, asset_code);
        }
        else
        {
            update_asset_position(investor, asset_code, 0);
            log_message(LOG_INFO, "Investor %s sells all stocks of %s\n", investor->name, asset_code);
        }

        // Dorme por tempo aleatório (simulação de intervalos variáveis)
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
            log_message(LOG_INFO, "Escalonando investidor: %s\n", investor->name);
            pthread_create(&investor->id, NULL, investor_lifecycle, (void *)investor);
            pthread_join(investor->id, NULL);
            enqueue_investor(&queue, investor);
        }

        sleep(INVESTOR_LIFETIME);
    }

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
