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

/**
 * @brief Inicializa uma estrutura `Investor` com base nos parâmetros fornecidos.
 *
 * Esta função cria e inicializa uma estrutura `Investor` com um identificador de thread e nome fornecidos,
 * e define as posições de ativos como vazias.
 *
 * @param investor Ponteiro para a estrutura `Investor` que será inicializada.
 * @param id Identificador único do investidor (pthread_t).
 * @param name Nome do investidor (string).
 *
 * @return Retorna 0 (SUCCESS) em caso de sucesso, ou um código de erro caso os dados sejam inválidos.
 *
 * @note O nome do investidor não pode exceder o comprimento máximo definido por `MAX_NAME_LENGTH`.
 */
int create_investor(Investor *investor, pthread_t id, const char *name);

/**
 * @brief Adiciona uma nova posição de ativo ao portfólio do investidor.
 * 
 * Esta função insere uma nova posição de ativo ao array de posições do investidor. A função falha se o número
 * máximo de posições já tiver sido atingido ou se a posição for inválida.
 * 
 * @param investor Ponteiro para a estrutura `Investor`.
 * @param asset_position Ponteiro para a estrutura `Position` que representa a posição do ativo a ser adicionada.
 * 
 * @return Retorna 0 (SUCCESS) em caso de sucesso, ou um código de erro caso a adição falhe.
 * 
 * @note O investidor só pode ter até `MAX_POSITIONS` ativos no seu portfólio.
 */
int add_asset_position(Investor *investor, Position *asset_position);

/**
 * @brief Atualiza a quantidade de ações de uma posição existente.
 * 
 * Esta função procura a posição do ativo no portfólio do investidor com base no código do ativo e atualiza a quantidade de ações
 * associada àquela posição.
 * 
 * @param investor Ponteiro para a estrutura `Investor`.
 * @param asset_code Código do ativo cujas ações serão atualizadas.
 * @param shares Nova quantidade de ações a ser atribuída à posição.
 * 
 * @return Retorna 0 (SUCCESS) se a atualização for bem-sucedida, ou um código de erro caso contrário (e.g., se o ativo não for encontrado).
 * 
 * @note A função falha se o ativo não estiver presente no portfólio.
 */
int update_asset_position(Investor *investor, const char *asset_code, int shares);


/**
 * @brief Recupera uma posição de ativo com base no código do ativo.
 * 
 * Esta função retorna um ponteiro para a estrutura `Position` correspondente a um ativo no portfólio do investidor,
 * baseado no código do ativo.
 * 
 * @param investor Ponteiro para a estrutura `Investor`.
 * @param asset_code Código do ativo que será procurado.
 * 
 * @return Retorna um ponteiro para a posição do ativo, ou NULL se o ativo não for encontrado no portfólio.
 */
Position *get_asset_position(Investor *investor, const char *asset_code);

/**
 * @brief Encontra o índice de uma posição de ativo com base no código do ativo.
 * 
 * Esta função retorna o índice da posição de um ativo no array de posições do investidor. Se o ativo não for encontrado,
 * a função retorna um valor negativo.
 * 
 * @param investor Ponteiro para a estrutura `Investor`.
 * @param asset_code Código do ativo a ser procurado.
 * 
 * @return Retorna o índice da posição no array `positions` se o ativo for encontrado, ou -1 caso contrário.
 */
int get_position_index(Investor *investor, char asset_code[MAX_ASSET_CODE_LENGTH]);

#endif // INVESTOR_H
