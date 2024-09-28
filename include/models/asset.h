#ifndef ASSET_H
#define ASSET_H

#define MAX_CODE_LENGTH 6
#define MAX_COMPANY_NAME_LENGTH 50
#define NUM_ASSETS 3

typedef struct
{
    char code[MAX_CODE_LENGTH];
    char company_name[MAX_COMPANY_NAME_LENGTH];
    int market_volume;
} Asset;

extern Asset assets[NUM_ASSETS];
extern const char *ASSETS_CODE[NUM_ASSETS];
extern const char *COMPANY_NAMES[NUM_ASSETS];

int create_asset(Asset *asset, const char *code, const char *company_name, int market_volume);

#endif // ASSET_H
