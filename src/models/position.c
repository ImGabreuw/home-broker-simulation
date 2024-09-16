#include "position.h"

#include <string.h>

#include "error_codes.h"
#include "log.h"

int create_position(Position *position, const char *asset_code, int shares) {
    if (asset_code == NULL || strlen(asset_code) >= MAX_ASSET_CODE_LENGTH) {
        log_message(LOG_WARNING, "Invalid asset code");
        return ERR_VALIDATION;
    }

    if (shares < 0) {
        log_message(LOG_WARNING, "Shares cannot be negative");
        return ERR_VALIDATION;
    }

    strncpy(position->asset_code, asset_code, MAX_ASSET_CODE_LENGTH - 1);
    position->asset_code[MAX_ASSET_CODE_LENGTH - 1] = '\0';
    position->shares = shares;

    log_message(LOG_INFO, "Position created: Asset Code: %s, Shares: %d", position->asset_code, position->shares);

    return SUCCESS;
}
