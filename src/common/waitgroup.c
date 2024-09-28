#include "waitgroup.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "error_codes.h"

int init_waitgroup(Waitgroup *wg)
{
    if (wg == NULL)
    {
        return ERR_MEMORY_ALLOCATION;
    }

    wg->waitGroupSize = 0;

    int ok = pthread_mutex_init(&(wg->mutex), NULL);
    if (ok != 0)
    {
        return ERR_MEMORY_ALLOCATION;
    }

    ok = pthread_cond_init(&(wg->cond), NULL);
    if (ok != 0)
    {
        pthread_mutex_destroy(&(wg->mutex));
        return ERR_MEMORY_ALLOCATION;
    }

    return SUCCESS;
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

    if (wg->waitGroupSize <= 0)
    {
        pthread_cond_signal(&(wg->cond)); // Sinaliza que a contagem chegou a 0
    }

    pthread_mutex_unlock(&(wg->mutex));
}

void wait_waitgroup(Waitgroup *wg)
{
    if (wg == NULL)
    {
        return;
    }

    pthread_mutex_lock(&(wg->mutex));

    while (wg->waitGroupSize > 0)
    {
        pthread_cond_wait(&(wg->cond), &(wg->mutex)); // Espera até que o waitGroupSize seja 0
    }

    pthread_mutex_unlock(&(wg->mutex));
}

void destroy_waitgroup(Waitgroup *wg)
{
    if (wg == NULL)
    {
        return;
    }

    pthread_mutex_destroy(&(wg->mutex));
    pthread_cond_destroy(&(wg->cond));
}
