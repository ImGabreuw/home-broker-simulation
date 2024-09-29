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

/**
 * @var assets
 * @brief Array global de ativos pré-definidos.
 */
extern Asset assets[NUM_ASSETS];

/**
 * @var ASSETS_CODE
 * @brief Array de strings com os códigos dos ativos.
 */
extern const char *ASSETS_CODE[NUM_ASSETS];

/**
 * @var COMPANY_NAMES
 * @brief Array de strings com os nomes das empresas.
 */
extern const char *COMPANY_NAMES[NUM_ASSETS];

/**
 * @brief Cria e inicializa um ativo com base nos parâmetros fornecidos.
 *
 * Esta função recebe os parâmetros necessários para inicializar um ativo financeiro e armazena
 * os dados em uma estrutura `Asset` fornecida.
 *
 * @param asset Ponteiro para a estrutura Asset a ser inicializada.
 * @param code Código do ativo (e.g., "PETR4").
 * @param company_name Nome da empresa que emite o ativo (e.g., "Petrobras").
 * @param market_volume Volume de mercado fictício do ativo, representando o número de ações disponíveis.
 *
 * @return Retorna 0 (SUCCESS) em caso de sucesso, ou um código de erro (ERR_VALIDATION) caso os dados sejam inválidos.
 *
 * @note A função verifica se o código do ativo, o nome da empresa e o volume de mercado estão dentro dos limites e são válidos.
 */
int create_asset(Asset *asset, const char *code, const char *company_name, int market_volume);

#endif // ASSET_H
