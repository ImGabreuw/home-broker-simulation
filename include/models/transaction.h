#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <time.h>

#include "order.h"

// Fix circular typedef dependency (https://stackoverflow.com/questions/888386/resolve-circular-typedef-dependency)
typedef struct Transaction Transaction;
typedef struct Order Order;

struct Transaction
{
    int id;
    Order *selling_order;
    Order *buying_order;
    int shares;
    double price;
    double total; ///< Valor total da transação (shares * price).
    time_t created_at;
};

/**
 * @brief Cria uma nova transação entre uma ordem de compra e uma de venda.
 *
 * Essa função inicializa uma estrutura `Transaction` com os detalhes da transação, incluindo as ordens de venda e compra,
 * a quantidade de ações e o preço por ação.
 *
 * @param transaction Ponteiro para a estrutura `Transaction` que será preenchida.
 * @param selling_order Ponteiro para a ordem de venda associada à transação.
 * @param buying_order Ponteiro para a ordem de compra associada à transação.
 *
 * @return Retorna 0 (SUCCESS) em caso de sucesso ou um código de erro em caso de falha.
 *
 * @note A função não calcula o valor total da transação automaticamente; para isso, utilize `calculate_total()`.
 */
int create_transaction(Transaction *transaction, Order *selling_order, Order *buying_order);

/**
 * @brief Calcula o valor total da transação.
 *
 * Essa função calcula o valor total da transação multiplicando a quantidade de ações pelo preço por ação.
 *
 * @param transaction Ponteiro para a estrutura `Transaction` cuja total será calculado.
 *
 * @note O valor total é armazenado no campo `total` da estrutura `Transaction`.
 */
void calculate_total(Transaction *transaction);

/**
 * @brief Fecha a ordem de compra associada a uma transação.
 *
 * Essa função define o status da ordem de compra relacionada como `CLOSE`, indicando que ela foi totalmente executada.
 *
 * @param transaction Ponteiro para a estrutura `Transaction` associada à ordem de compra a ser fechada.
 */
void close_buy_order(Transaction *transaction);

/**
 * @brief Fecha a ordem de venda associada a uma transação.
 *
 * Essa função define o status da ordem de venda relacionada como `CLOSE`, indicando que ela foi totalmente executada.
 *
 * @param transaction Ponteiro para a estrutura `Transaction` associada à ordem de venda a ser fechada.
 */
void close_sell_order(Transaction *transaction);

/**
 * @brief Adiciona ações pendentes à ordem de compra.
 *
 * Essa função adiciona um número especificado de ações ao campo de ações pendentes (`pending_shares`) da ordem de compra associada à transação.
 *
 * @param transaction Ponteiro para a estrutura `Transaction` associada à ordem de compra.
 * @param shares Número de ações a serem adicionadas às ações pendentes.
 */
void add_buy_order_pending_shares(Transaction *transaction, int shares);

/**
 * @brief Adiciona ações pendentes à ordem de venda.
 *
 * Essa função adiciona um número especificado de ações ao campo de ações pendentes (`pending_shares`) da ordem de venda associada à transação.
 *
 * @param transaction Ponteiro para a estrutura `Transaction` associada à ordem de venda.
 * @param shares Número de ações a serem adicionadas às ações pendentes.
 */
void add_sell_order_pending_shares(Transaction *transaction, int shares);

#endif // TRANSACTION_H
