#ifndef INVESTOR_MANAGER_SERVICES_H
#define INVESTOR_MANAGER_SERVICES_H

#include <pthread.h>

#include "investor.h"

#define MAX_INVESTORS 20 
#define GROUP_SIZE 5    

void start_investors_management();

// Função que simula as operações de compra e venda de um investidor
void *investor_lifecycle(void *arg);

#endif // INVESTOR_MANAGER_SERVICES_H
