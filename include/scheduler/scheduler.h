#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <pthread.h>

#include "investor.h"

#define INVESTOR_LIFETIME 2 // Tempo de execução para cada investidor (em segundos)
#define MAX_OPERATIONS 5

/**
 * @brief Simula o escalonamento de múltiplos investidores.
 *
 * Esta função simula o escalonamento de `num_investors` investidores, criando uma thread para cada investidor, que executará um ciclo de vida simulado.
 * Cada investidor terá um tempo de execução limitado definido por `INVESTOR_LIFETIME`.
 *
 * @param investors Ponteiro para um array de investidores a serem escalonados.
 * @param num_investors Número de investidores a serem escalonados.
 */
void simulate_investor_scheduling(Investor *investors, int num_investors);

/**
 * @brief Função de ciclo de vida de um investidor.
 *
 * Esta função simula o ciclo de vida de um investidor. Cada investidor realiza um conjunto de operações, conforme definido por `MAX_OPERATIONS`.
 * É executada em uma thread separada para cada investidor e inclui um loop que simula a vida útil do investidor por `INVESTOR_LIFETIME` segundos.
 *
 * @param arg Ponteiro para a estrutura `Investor` associada à thread.
 * @return Retorna `NULL` quando a thread termina.
 */
void *investor_lifecycle(void *arg);

/**
 * @brief Função da thread do escalonador.
 *
 * Esta função é responsável por coordenar o funcionamento de um conjunto de investidores, alocando tempo para cada um, como em um escalonador de sistema operacional.
 * Ela gerencia o início e fim do ciclo de vida de cada investidor, chamando a função `investor_lifecycle` em threads separadas.
 *
 * @param arg Argumento passado para a thread do escalonador. Tipicamente, este argumento inclui informações sobre o conjunto de investidores.
 * @return Retorna `NULL` quando o escalonador termina sua execução.
 */
void *scheduler_thread(void *arg);

#endif // SCHEDULER_H
