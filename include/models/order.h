#ifndef ORDER_H
#define ORDER_H

#include "investor.h"
#include "asset.h"
#include "transaction.h"

/**
 * @enum OrderAction
 * @brief Define o tipo de ação de uma ordem.
 *
 * Esse enum é utilizado para indicar se uma ordem é de compra (BUY) ou de venda (SELL).
 */
typedef enum
{
    BUY,
    SELL
} OrderAction;

/**
 * @enum OrderStatus
 * @brief Define o estado atual de uma ordem.
 *
 * Esse enum é utilizado para indicar o status de uma ordem no mercado, podendo ser:
 * - OPEN: Ordem aberta e aguardando execução.
 * - CLOSE: Ordem concluída.
 * - CANCELED: Ordem cancelada antes da execução.
 */
typedef enum
{
    OPEN,
    CLOSE,
    CANCELED
} OrderStatus;

// Fix circular typedef dependency (https://stackoverflow.com/questions/888386/resolve-circular-typedef-dependency)
typedef struct Transaction Transaction;
typedef struct Order Order;

struct Order
{
    int id;
    Investor *investor;
    Asset *asset;
    int shares;         ///< Quantidade total de ações na ordem.
    int pending_shares; ///< Quantidade de ações pendentes de execução
    double price;
    OrderAction action;
    OrderStatus status;
    Transaction *transactions;
};

/**
 * @brief Emite uma nova ordem para o mercado.
 *
 * A função cria uma nova ordem com as informações fornecidas, incluindo o investidor, ativo, quantidade de ações, preço e tipo de ação.
 * A string `order_type_str` é convertida para o tipo de ação correspondente (`BUY` ou `SELL`).
 *
 * @param order Ponteiro para a estrutura `Order` que será preenchida com os detalhes da nova ordem.
 * @param id Identificador único para a nova ordem.
 * @param investor Ponteiro para o investidor que está emitindo a ordem.
 * @param asset Ponteiro para o ativo relacionado à ordem.
 * @param shares Quantidade de ações na ordem.
 * @param price Preço por ação definido para a ordem.
 * @param order_type_str String representando o tipo de ordem ("BUY" ou "SELL").
 *
 * @return Retorna 0 (SUCCESS) em caso de sucesso, ou um código de erro em caso de falha na criação da ordem.
 *
 * @note A função converte a string `order_type_str` para o enum correspondente `OrderAction`.
 */
int emit_order(Order *order, int id, Investor *investor, Asset *asset, int shares, double price, const char *order_type_str);

/**
 * @brief Obtém o nome da ação da ordem em formato de string.
 *
 * Essa função traduz o tipo de ação (compra ou venda) de uma ordem para uma string legível.
 *
 * @param order Ponteiro para a estrutura `Order`.
 * @param action_name Ponteiro para a string onde será armazenado o nome da ação ("BUY" ou "SELL").
 *
 * @note O nome da ação será armazenado em `action_name`, que deve ter espaço suficiente para armazenar a string resultante.
 */
void get_action_name(Order *order, char *action_name);

#endif // ORDER_H
