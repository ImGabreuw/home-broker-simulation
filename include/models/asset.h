#ifndef ASSET_H
#define ASSET_H

#define MAX_CODE_LENGTH 6
#define MAX_COMPANY_NAME_LENGTH 50

typedef struct {
    char code[MAX_CODE_LENGTH];
    char company_name[MAX_COMPANY_NAME_LENGTH];
    int market_volume;
} Asset;

int create_asset(Asset *asset, const char *code, const char *company_name, int market_volume);

#endif // ASSET_H
