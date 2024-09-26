#include "waitgroup.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int init_waitgroup(Waitgroup *wg)
{
    if (wg == NULL)
    {
        return -1; // Erro: ponteiro nulo
    }

    wg->waitGroupSize = 0;

    int ok = pthread_mutex_init(&(wg->mutex), NULL);
    if (ok != 0)
    {
        return -1; // Erro ao inicializar mutex
    }

    return 0; // Sucesso
}

void add_waitgroup(Waitgroup *wg, int size)
{
    if (wg == NULL)
    {
        return;
    }

    pthread_mutex_lock(&(wg->mutex));
    wg->waitGroupSize += size;
    pthread_mutex_unlock(&(wg->mutex));
}

void done_waitgroup(Waitgroup *wg)
{
    if (wg == NULL)
    {
        return;
    }

    pthread_mutex_lock(&(wg->mutex));
    wg->waitGroupSize--;
    pthread_mutex_unlock(&(wg->mutex));
}

void wait_waitgroup(Waitgroup *wg)
{
    if (wg == NULL)
    {
        return;
    }

    while (1)
    {
        pthread_mutex_lock(&(wg->mutex));
        if (wg->waitGroupSize <= 0)
        {
            pthread_mutex_unlock(&(wg->mutex));
            break; 
        }
        pthread_mutex_unlock(&(wg->mutex));
        usleep(100);
    }
}
