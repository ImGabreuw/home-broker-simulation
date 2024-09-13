#include "stock.h"

#include <stdio.h>
#include <string.h>

#include "log.h"
#include "error_codes.h"

int create_stock(stock *stock, char *id, char *companyName, double value, int availableAmount)
{
    if (id == NULL || strlen(id) == 0)
    {
        log_message(LOG_WARNING, "Invalid stock ID\n");
        return ERR_VALIDATION;
    }

    if (companyName == NULL || strlen(companyName) == 0)
    {
        log_message(LOG_WARNING, "Invalid company name\n");
        return ERR_VALIDATION;
    }

    if (value <= 0)
    {
        log_message(LOG_WARNING, "Stock price must be greater than 0\n");
        return ERR_VALIDATION;
    }

    if (availableAmount < 0)
    {
        log_message(LOG_WARNING, "Available amount cannot be negative\n");
        return ERR_VALIDATION;
    }

    stock->code = id;
    stock->companyName = companyName;
    stock->currentPrice = value;
    stock->availableAmount = availableAmount;
    stock->isAvailable = true;

    log_message(LOG_INFO, "Stock created: %s (%s), Price: %.2f, Available: %d\n", id, companyName, value, availableAmount);
    
    return SUCCESS;
}
