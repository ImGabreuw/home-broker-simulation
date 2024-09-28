#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <pthread.h>

#include "investor.h"

#define INVESTOR_LIFETIME 2 // Tempo de execução para cada investidor (em segundos)
#define MAX_OPERATIONS 5

void simulate_investor_scheduling(Investor *investors, int num_investors);

void *investor_lifecycle(void *arg);

void *scheduler_thread(void *arg);

#endif // SCHEDULER_H
