#include "order.h"

#include <stdio.h>
#include <string.h>

#include "log.h"
#include "test_utils.h"

void test_emit_order()
{
    Investor investor;
    Asset asset;
    pthread_t id = pthread_self();

    create_investor(&investor, id, "John Doe");
    strncpy(asset.code, "AAPL", MAX_ASSET_CODE_LENGTH);
    strncpy(asset.company_name, "Apple Inc.", MAX_COMPANY_NAME_LENGTH);
    asset.market_volume = 100000;

    Order order;
    int result = emit_order(&order, 1, &investor, &asset, 100, 150.0, "BUY");

    if (result == SUCCESS && order.shares == 100 && order.price == 150.0 && order.action == BUY)
    {
        test_passed("test_emit_order");
    }
    else
    {
        test_failed("test_emit_order");
    }
}

void test_emit_invalid_order()
{
    Investor investor;
    Asset asset;
    pthread_t id = pthread_self();

    create_investor(&investor, id, "John Doe");
    strncpy(asset.code, "AAPL", MAX_ASSET_CODE_LENGTH);
    strncpy(asset.company_name, "Apple Inc.", MAX_COMPANY_NAME_LENGTH);
    asset.market_volume = 100000;

    Order order;
    int result = emit_order(&order, 1, &investor, &asset, 0, 150.0, "BUY");

    if (result == ERR_VALIDATION)
    {
        test_passed("test_emit_invalid_order");
    }
    else
    {
        test_failed("test_emit_invalid_order");
    }
}

int main()
{
    test_emit_order();
    test_emit_invalid_order();

    return 0;
}
