#ifndef INVESTOR_H
#define INVESTOR_H

#include <pthread.h>

#include "position.h"

#define MAX_POSITIONS 10
#define MAX_NAME_LENGTH 60

typedef struct
{
    pthread_t id;
    char name[MAX_NAME_LENGTH];
    Position positions[MAX_POSITIONS];
} Investor;

int create_investor(Investor *investor, pthread_t id, const char *name);

int add_asset_position(Investor *investor, Position *asset_position);

int update_asset_position(Investor *investor, const char *asset_code, int shares);

Position *get_asset_position(Investor *investor, const char *asset_code);

#endif // INVESTOR_H
