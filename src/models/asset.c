#include "asset.h"

#include <string.h>

int create_asset(Asset *asset, const char *code, const char *company_name, int market_volume) {
    if (code == NULL || strlen(code) >= MAX_CODE_LENGTH) {
        log_message(LOG_WARNING, "Invalid asset code");
        return ERR_VALIDATION;
    }

    if (company_name == NULL || strlen(company_name) >= MAX_COMPANY_NAME_LENGTH) {
        log_message(LOG_WARNING, "Invalid company name");
        return ERR_VALIDATION;
    }

    if (market_volume < 0) {
        log_message(LOG_WARNING, "Market volume cannot be negative");
        return ERR_VALIDATION;
    }

    strncpy(asset->code, code, MAX_CODE_LENGTH - 1);
    asset->code[MAX_CODE_LENGTH - 1] = '\0';

    strncpy(asset->company_name, company_name, MAX_COMPANY_NAME_LENGTH - 1);
    asset->company_name[MAX_COMPANY_NAME_LENGTH - 1] = '\0';

    asset->market_volume = market_volume;

    log_message(LOG_INFO, "Asset created: Code: %s, Company: %s, Market Volume: %d", asset->code, asset->company_name, asset->market_volume);

    return SUCCESS;
}
