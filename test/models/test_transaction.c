#include "transaction.h"

#include <stdio.h>
#include <string.h>

#include "log.h"
#include "test_utils.h"

void test_create_transaction()
{
    Investor investor;
    Asset asset;
    pthread_t id = pthread_self();

    create_investor(&investor, id, "John Doe");
    strncpy(asset.code, "AAPL", 5);
    strncpy(asset.company_name, "Apple Inc.", MAX_COMPANY_NAME_LENGTH);
    asset.market_volume = 100000;

    Order buy_order, sell_order;
    emit_order(&buy_order, 1, &investor, &asset, 100, 150.0, "BUY");
    emit_order(&sell_order, 2, &investor, &asset, 100, 150.0, "SELL");

    Transaction transaction;
    int result = create_transaction(&transaction, &sell_order, &buy_order, 100, 150.0);

    if (result == SUCCESS && transaction.total == 15000.0)
    {
        test_passed("test_create_transaction");
    }
    else
    {
        test_failed("test_create_transaction");
    }
}

void test_close_orders()
{
    Investor investor;
    Asset asset;
    pthread_t id = pthread_self();

    create_investor(&investor, id, "John Doe");
    strncpy(asset.code, "AAPL", 5);
    strncpy(asset.company_name, "Apple Inc.", MAX_COMPANY_NAME_LENGTH);
    asset.market_volume = 100000;

    Order buy_order, sell_order;
    emit_order(&buy_order, 1, &investor, &asset, 100, 150.0, "BUY");
    emit_order(&sell_order, 2, &investor, &asset, 100, 150.0, "SELL");

    Transaction transaction;
    create_transaction(&transaction, &sell_order, &buy_order, 100, 150.0);

    close_buy_order(&transaction);
    close_sell_order(&transaction);

    if (buy_order.status == CLOSE && sell_order.status == CLOSE)
    {
        test_passed("test_close_orders");
    }
    else
    {
        test_failed("test_close_orders");
    }
}

int main()
{
    test_create_transaction();
    test_close_orders();

    return 0;
}
