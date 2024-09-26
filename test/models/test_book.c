#include "book.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "asset.h"
#include "order_queue.h"
#include "log.h"
#include "error_codes.h"
#include "test_utils.h"

void test_trade_matching_orders()
{
    Book book;
    OrderQueue input_queue, output_queue;
    Waitgroup wg;

    // Inicializa as filas de pedidos
    init_order_queue(&input_queue);
    init_order_queue(&output_queue);

    // Inicializa o waitgroup
    if (init_waitgroup(&wg) != 0)
    {
        log_message(LOG_ERROR, "Failed to initialize waitgroup.");
        return;
    }

    init_book(&book, &input_queue, &output_queue, &wg);

    Asset asset;
    create_asset(&asset, "A1234", "Example Company", 10000);

    Order buy_order, sell_order;
    buy_order.asset = &asset;
    buy_order.action = BUY;
    buy_order.shares = 100;
    buy_order.price = 50.0;
    buy_order.pending_shares = 100;

    sell_order.asset = &asset;
    sell_order.action = SELL;
    sell_order.shares = 100;
    sell_order.price = 50.0;
    sell_order.pending_shares = 100;

    enqueue_order(&input_queue, &buy_order);
    enqueue_order(&input_queue, &sell_order);

    // Sinaliza o WaitGroup para que a função trade possa ser chamada
    done_waitgroup(&wg);
    trade(&book);

    if (size(&output_queue) == 2)
    {
        test_passed("test_trade_matching_orders");
    }
    else
    {
        test_failed("test_trade_matching_orders");
    }
}

void test_partial_shares()
{
    Book book;
    OrderQueue input_queue, output_queue;
    Waitgroup wg;

    // Inicializa as filas de pedidos
    init_order_queue(&input_queue);
    init_order_queue(&output_queue);

    // Inicializa o waitgroup
    if (init_waitgroup(&wg) != 0)
    {
        log_message(LOG_ERROR, "Failed to initialize waitgroup.");
        return;
    }

    init_book(&book, &input_queue, &output_queue, &wg);

    Asset asset;
    int result = create_asset(&asset, "A1234", "Example Company", 10000);

    if (result == ERR_VALIDATION)
    {
        log_message(LOG_WARNING, "Failed to create asset.");
        return;
    }

    Order buy_order, sell_order;
    buy_order.asset = &asset;
    buy_order.action = BUY;
    buy_order.shares = 100;
    buy_order.price = 50.0;
    buy_order.pending_shares = 50; // Definindo ações pendentes menores

    sell_order.asset = &asset;
    sell_order.action = SELL;
    sell_order.shares = 100;
    sell_order.price = 50.0;
    sell_order.pending_shares = 100;

    enqueue_order(&input_queue, &buy_order);
    enqueue_order(&input_queue, &sell_order);

    done_waitgroup(&wg);
    trade(&book);

    if (sell_order.pending_shares == 50 && buy_order.pending_shares == 0)
    {
        test_passed("test_partial_shares");
    }
    else
    {
        test_failed("test_partial_shares");
    }
}

int main()
{
    log_init("log_test_book.txt");

    test_trade_matching_orders();
    // test_partial_shares();

    log_cleanup();

    return 0;
}
