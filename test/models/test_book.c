#include "book.h"

#include <pthread.h>
#include <string.h>

#include "log.h"
#include "error_codes.h"
#include "test_utils.h"

void *trade_thread(void *arg)
{
    Book *book = (Book *)arg;
    trade(book);
    done_waitgroup(book->wg);
    return NULL;
}

void test_trade_matching_orders()
{
    Book book;
    OrderQueue in, out;
    Waitgroup wg;

    if (init_waitgroup(&wg) != 0)
    {
        log_message(LOG_ERROR, "Failed to initialize waitgroup.");
        return;
    }

    init_book(&book, &in, &out, &wg);

    Asset asset;
    create_asset(&asset, "VALE3", "VALE S.A.", 10000);

    Investor investor1;
    pthread_t investor1_id = pthread_self();
    if (create_investor(&investor1, investor1_id, "Investor 1") != SUCCESS)
    {
        log_message(LOG_ERROR, "Failed to create investor 1.");
        return;
    }

    Investor investor2;
    pthread_t investor2_id = pthread_self();
    if (create_investor(&investor2, investor2_id, "Investor 2") != SUCCESS)
    {
        log_message(LOG_ERROR, "Failed to create investor 2.");
        return;
    }

    Position selling_position;
    strncpy(selling_position.asset_code, asset.code, MAX_ASSET_CODE_LENGTH);
    selling_position.shares = 100;
    add_asset_position(&investor2, &selling_position);

    Order buy_order;
    if (emit_order(&buy_order, 1, &investor1, &asset, 100, 50.0, "BUY") != SUCCESS)
    {
        log_message(LOG_ERROR, "Failed to create buy order.");
        return;
    }
    enqueue_order(&in, &buy_order);

    Order sell_order;
    if (emit_order(&sell_order, 2, &investor2, &asset, 100, 50.0, "SELL") != SUCCESS)
    {
        log_message(LOG_ERROR, "Failed to create sell order.");
        return;
    }
    enqueue_order(&in, &sell_order);

    add_waitgroup(&wg, 1);

    pthread_t trade_thread_id;
    if (pthread_create(&trade_thread_id, NULL, trade_thread, &book) != 0)
    {
        log_message(LOG_ERROR, "Failed to create trade thread.");
        return;
    }

    pthread_join(trade_thread_id, NULL);

    wait_waitgroup(&wg);

    if (size(&out) == 2)
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
    OrderQueue in, out;
    Waitgroup wg;

    if (init_waitgroup(&wg) != 0)
    {
        log_message(LOG_ERROR, "Failed to initialize waitgroup.");
        return;
    }

    init_book(&book, &in, &out, &wg);

    Asset asset;
    if (create_asset(&asset, "VALE3", "VALE S.A.", 10000) == ERR_VALIDATION)
    {
        log_message(LOG_WARNING, "Failed to create asset.");
        return;
    }

    Order buy_order, sell_order;
    buy_order.asset = &asset;
    buy_order.action = BUY;
    buy_order.shares = 100;
    buy_order.price = 50.0;
    buy_order.pending_shares = 50;

    sell_order.asset = &asset;
    sell_order.action = SELL;
    sell_order.shares = 100;
    sell_order.price = 50.0;
    sell_order.pending_shares = 100;

    enqueue_order(&in, &buy_order);
    enqueue_order(&in, &sell_order);

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

void test_no_match()
{
    Book book;
    OrderQueue in, out;
    Waitgroup wg;

    if (init_waitgroup(&wg) != 0)
    {
        log_message(LOG_ERROR, "Failed to initialize waitgroup.");
        return;
    }

    init_book(&book, &in, &out, &wg);

    Asset asset;
    if (create_asset(&asset, "VALE3", "VALE S.A.", 10000) == ERR_VALIDATION)
    {
        log_message(LOG_WARNING, "Failed to create asset.");
        return;
    }

    Order sell_order;
    sell_order.asset = &asset;
    sell_order.action = SELL;
    sell_order.shares = 3;
    sell_order.price = 6.0; 
    sell_order.pending_shares = 3;

    enqueue_order(&in, &sell_order);
    done_waitgroup(&wg);
    trade(&book);

    if (sell_order.pending_shares == 3)
    {
        test_passed("test_no_match");
    }
    else
    {
        test_failed("test_no_match");
    }
}

int main()
{
    log_init("log_test_book.txt");

    test_trade_matching_orders();
    // test_partial_shares();
    // test_no_match();

    log_cleanup();

    return 0;
}
