#include "investor.h"

#include <string.h>
#include <stdio.h>

#include "log.h"
#include "error_codes.h"

int create_investor(Investor *investor, pthread_t id, const char *name)
{
    if (name == NULL || strlen(name) >= MAX_NAME_LENGTH)
    {
        log_message(LOG_WARNING, "Invalid name");
        return ERR_VALIDATION;
    }

    investor->id = id;
    strncpy(investor->name, name, MAX_NAME_LENGTH - 1);
    investor->name[MAX_NAME_LENGTH - 1] = '\0';

    for (int i = 0; i < MAX_POSITIONS; i++)
    {
        investor->positions[i].shares = 0;
        investor->positions[i].asset_code[0] = '\0';
    }

    log_message(LOG_INFO, "Investor created: ID: %lu, Name: %s", id, investor->name);
    return SUCCESS;
}

int add_asset_position(Investor *investor, Position *asset_position)
{
    for (int i = 0; i < MAX_POSITIONS; i++)
    {
        if (investor->positions[i].shares == 0 && investor->positions[i].asset_code[0] == '\0')
        {
            investor->positions[i] = *asset_position;

            log_message(LOG_INFO, "Position added: Asset Code: %s, Shares: %d", asset_position->asset_code, asset_position->shares);
            return SUCCESS;
        }
    }

    log_message(LOG_WARNING, "No available slot for new position");
    return ERR_VALIDATION;
}

int update_asset_position(Investor *investor, const char *asset_code, int shares)
{
    if (asset_code == NULL || strlen(asset_code) >= 6)
    {
        log_message(LOG_WARNING, "Invalid asset code");
        return ERR_VALIDATION;
    }

    if (shares < 0)
    {
        log_message(LOG_WARNING, "Shares cannot be negative");
        return ERR_VALIDATION;
    }

    for (int i = 0; i < MAX_POSITIONS; i++)
    {
        if (strcmp(investor->positions[i].asset_code, asset_code) == 0)
        {
            investor->positions[i].shares = shares;
            log_message(LOG_INFO, "Position updated: Asset Code: %s, Shares: %d", asset_code, shares);
            return SUCCESS;
        }
    }

    log_message(LOG_WARNING, "Position with asset code %s not found", asset_code);
    return ERR_NOT_FOUND;
}

Position *get_asset_position(Investor *investor, const char *asset_code)
{
    if (asset_code == NULL || strlen(asset_code) >= 6)
    {
        log_message(LOG_WARNING, "Invalid asset code");
        return NULL;
    }

    for (int i = 0; i < MAX_POSITIONS; i++)
    {
        if (strcmp(investor->positions[i].asset_code, asset_code) == 0)
        {
            return &investor->positions[i];
        }
    }

    log_message(LOG_WARNING, "Position with asset code %s not found", asset_code);
    return NULL;
}
