#include "investor.h"

#include <stdio.h>
#include <string.h>

#include "log.h"

void create_investor(investor *investor, pthread_t id, char *name, double currentBalance)
{
    investor->id = id;
    investor->name = name;
    investor->currentBalance = currentBalance;
    memset(investor->ownedStocks, 0, sizeof(investor->ownedStocks));

    log_message(LOG_INFO, "Investor created: %s stock, ID: %ld, Balance: %.2f\n", name, id, currentBalance);
}

void add_stock_to_investor(investor *investor, stock *stock)
{
    for (int i = 0; i < MAX_STOCKS; i++)
    {
        if (investor->ownedStocks[i].code == NULL)
        {
            investor->ownedStocks[i] = *stock;
            log_message(LOG_INFO, "Stock %s added to investor %s stock portfolio\n", stock->code, investor->name);
            return;
        }
    }

    log_message(LOG_INFO, "Investor %s stock portfolio is full\n", investor->name);
}

void remove_stock_from_investor(investor *investor, stock *stock)
{
    for (int i = 0; i < MAX_STOCKS; i++)
    {
        if (investor->ownedStocks[i].code != NULL && strcmp(investor->ownedStocks[i].code, stock->code) == 0)
        {
            investor->ownedStocks[i].code = NULL;
            log_message(LOG_INFO, "Stock %s removed from investor %s stock portfolio\n", stock->code, investor->name);
            return;
        }
    }

    log_message(LOG_INFO, "Stock %s not found in investor %s stock portfolio\n", stock->code, investor->name);
}
