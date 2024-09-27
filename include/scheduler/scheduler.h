#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <pthread.h>

#include "investor.h"

#define INVESTOR_LIFETIME 2 // Tempo de execução para cada investidor (em segundos)
#define MAX_OPERATIONS 5

#define NUM_ASSETS 10
static const char *ASSETS[NUM_ASSETS] = {
    "PETR4",
    "VALE3",
    "ITUB4",
    "BBDC4",
    "ABEV3",
    "BBAS3",
    "WEGE3",
    "RENT3",
    "LREN3",
    "GGBR4"};

void simulate_investor_scheduling(Investor *investors, int num_investors);

void *investor_lifecycle(void *arg);

void *scheduler_thread(void *arg);

#endif // SCHEDULER_H
