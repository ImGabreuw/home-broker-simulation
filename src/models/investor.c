#include "investor.h"

#include <stdio.h>
#include <string.h>

#include "log.h"
#include "error_codes.h"

int create_investor(investor *investor, pthread_t id, char *name, double currentBalance)
{
    if (name == NULL || strlen(name) == 0)
    {
        log_message(LOG_WARNING, "Invalid investor name\n");
        return ERR_VALIDATION;
    }

    if (currentBalance < 0)
    {
        log_message(LOG_WARNING, "Investor balance cannot be negative\n");
        return ERR_VALIDATION;
    }

    investor->id = id;
    investor->name = name;
    investor->currentBalance = currentBalance;

    memset(investor->ownedStocks, 0, sizeof(investor->ownedStocks));

    log_message(LOG_INFO, "Investor created: %s, ID: %ld, Balance: %.2f\n", name, id, currentBalance);

    return SUCCESS;
}

int buy_stock(investor *investor, stock *stock)
{
    if (investor == NULL || stock == NULL)
    {
        log_message(LOG_WARNING, "Invalid investor or stock\n");
        return ERR_VALIDATION;
    }

    if (investor->currentBalance < stock->currentPrice)
    {
        log_message(LOG_WARNING, "Insufficient balance to buy stock %s\n", stock->code);
        return ERR_VALIDATION;
    }

    if (stock->availableAmount <= 0)
    {
        log_message(LOG_WARNING, "No available stocks to buy for %s\n", stock->code);
        return ERR_VALIDATION;
    }

    for (int i = 0; i < MAX_STOCKS; i++)
    {
        if (investor->ownedStocks[i].code == NULL)
        {
            investor->currentBalance -= stock->currentPrice;
            stock->availableAmount--;

            investor->ownedStocks[i] = *stock;

            log_message(LOG_INFO, "Stock %s added to investor %s stock portfolio. Balance: %.2f\n",
                        stock->code, investor->name, investor->currentBalance);
            return SUCCESS;
        }
    }

    log_message(LOG_INFO, "Investor %s stock portfolio is full\n", investor->name);
    return ERR_VALIDATION;
}

int sell_stock(investor *investor, stock *stock)
{
    if (investor == NULL || stock == NULL)
    {
        log_message(LOG_WARNING, "Invalid investor or stock\n");
        return ERR_VALIDATION;
    }

    for (int i = 0; i < MAX_STOCKS; i++)
    {
        if (investor->ownedStocks[i].code != NULL && strcmp(investor->ownedStocks[i].code, stock->code) == 0)
        {
            investor->currentBalance += stock->currentPrice;
            stock->availableAmount++;

            investor->ownedStocks[i].code = NULL;

            log_message(LOG_INFO, "Stock %s removed from investor %s stock portfolio. Balance: %.2f\n",
                        stock->code, investor->name, investor->currentBalance);
            return SUCCESS;
        }
    }

    log_message(LOG_WARNING, "Stock %s not found in investor %s stock portfolio\n", stock->code, investor->name);
    return ERR_VALIDATION;
}
