#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "log.h"
#include "book.h"
#include "error_codes.h"
#include "investor.h"
#include "scheduler.h"
#include "asset.h"

#define NUM_INVESTORS 5

Book book;
OrderQueue order_channel;

Asset assets[NUM_ASSETS];

void *trade_thread(void *arg)
{
    Book *book = (Book *)arg;
    trade(book);
    return NULL;
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    char log_filename[] = "home_broker_log.txt";
    log_init(log_filename);

    log_message(LOG_INFO, "Home Breaker started...");
    init_book(&book, &order_channel);

    pthread_t trade_thread_id;
    if (pthread_create(&trade_thread_id, NULL, trade_thread, &book) != 0)
    {
        log_message(LOG_ERROR, "Failed to create trade thread.");
        return ERR_MEMORY_ALLOCATION;
    }

    for (int i = 0; i < NUM_ASSETS; i++)
    {
        if (create_asset(&assets[i], ASSETS_CODE[i], COMPANY_NAMES[i], 1000) != SUCCESS)
        {
            printf("Failed to create asset %s\n", ASSETS_CODE[i]);
        }
    }

    Investor investors[NUM_INVESTORS];
    char *investor_names[NUM_INVESTORS] = {"Alice", "Bob", "Charlie", "David", "Eve"};

    for (int i = 0; i < NUM_INVESTORS; i++)
    {
        pthread_t investor_id = pthread_self() + i;
        Investor *investor = (Investor *)malloc(sizeof(Investor));
        if (investor == NULL)
        {
            log_message(LOG_ERROR, "Failed to allocate memory for new investor.");
            pthread_exit(NULL);
        }

        if (create_investor(investor, investor_id, investor_names[i]) != SUCCESS)
        {
            log_message(LOG_ERROR, "Failed to create investor: %s", investor_names[i]);
            return ERR_MEMORY_ALLOCATION;
        }

        for (int i = 0; i < NUM_ASSETS; i++)
        {
            Position *new_position = (Position *)malloc(sizeof(Position));
            
            if (new_position == NULL)
            {
                log_message(LOG_ERROR, "Failed to allocate memory for new position.");
                pthread_exit(NULL);
            }
            int shares = (rand() % 100) + 5001;
            new_position->shares = shares;

            strncpy(new_position->asset_code, ASSETS_CODE[i], MAX_ASSET_CODE_LENGTH);

            add_asset_position(investor, new_position);
        }

        investors[i] = *investor;
    }

    simulate_investor_scheduling(investors, NUM_INVESTORS);

    while (1) {
        sleep(1);
    }

    log_cleanup();

    return 0;
}