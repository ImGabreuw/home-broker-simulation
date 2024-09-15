#include "investor_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "system.h"

pthread_t investors[MAX_INVESTORS];
investor investors_data[MAX_INVESTORS];

void *investor_lifecycle(void *arg)
{
    investor *inv = (investor *)arg;

    int lifetime = 20 + rand() % 11;
    time_t start = time(NULL);

    while (time(NULL) - start < lifetime)
    {
        int action = rand() % 2; // 0 para comprar, 1 para vender
        int stock_index = rand() % MAX_STOCKS;

        // Simula a operação com um delay aleatório (2 a 5 segundos)
        int delay = 2 + rand() % 4;
        sleep(delay);

        if (action == 0)
        {
            buy_stock(inv, &stocks[stock_index]);
        }
        else
        {
            sell_stock(inv, &stocks[stock_index]);
        }
    }

    pthread_exit(NULL);
}

void start_investors_management()
{
    srand(time(NULL));

    for (int i = 0; i < MAX_INVESTORS; i++)
    {
        char name[20];
        sprintf(name, "Investor %d", i + 1);
        create_investor(&investors_data[i], investors[i], name, 1000.0 + rand() % 5000);
    }

    int current_group = 0;
    while (1)
    {
        // Lançar um grupo de investidores (de 5 em 5)
        for (int i = current_group; i < current_group + GROUP_SIZE; i++)
        {
            pthread_create(&investors[i], NULL, investor_lifecycle, &investors_data[i]);
        }

        // Esperar o grupo ativo completar o ciclo de vida (simulando quantum)
        sleep(30);

        // Esperar todos os threads do grupo ativo finalizarem
        for (int i = current_group; i < current_group + GROUP_SIZE; i++)
        {
            pthread_join(investors[i], NULL);
        }

        // Avançar para o próximo grupo de 5 investidores
        current_group = (current_group + GROUP_SIZE) % MAX_INVESTORS;
    }
}
