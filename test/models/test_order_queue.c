#include "order_queue.h"

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "order.h"
#include "log.h"
#include "error_codes.h"
#include "test_utils.h"

void test_init_queue()
{
    OrderQueue queue;
    int result = init_order_queue(&queue);

    if (result == SUCCESS && size(&queue) == 0)
    {
        test_passed("test_init_queue");
    }
    else
    {
        test_failed("test_init_queue");
    }
}

void test_enqueue_order()
{
    OrderQueue queue;
    init_order_queue(&queue);

    Asset asset;
    strcpy(asset.code, "A1");
    strcpy(asset.company_name, "Test Company");
    asset.market_volume = 1000;

    Order order;
    order.asset = &asset;
    order.action = BUY;
    order.shares = 100;
    order.price = 50.0;
    order.pending_shares = 100;

    enqueue_order(&queue, &order);

    if (size(&queue) == 1)
    {
        test_passed("test_enqueue_order");
    }
    else
    {
        test_failed("test_enqueue_order");
    }
}

void test_dequeue_order()
{
    OrderQueue queue;
    init_order_queue(&queue);

    Asset asset;
    strcpy(asset.code, "A1");
    strcpy(asset.company_name, "Test Company");
    asset.market_volume = 1000;

    Order order;
    order.asset = &asset;
    order.action = SELL;
    order.shares = 200;
    order.price = 70.0;
    order.pending_shares = 200;

    enqueue_order(&queue, &order);

    Order dequeued_order;
    int result = dequeue_order(&queue, &dequeued_order);

    if (result == SUCCESS && size(&queue) == 0 && dequeued_order.shares == 200)
    {
        test_passed("test_dequeue_order");
    }
    else
    {
        test_failed("test_dequeue_order");
    }
}

void test_queue_empty()
{
    OrderQueue queue;
    init_order_queue(&queue);

    Order dequeued_order;
    int result = dequeue_order(&queue, &dequeued_order);

    if (result == ERR_VALIDATION)
    {
        test_passed("test_queue_empty");
    }
    else
    {
        test_failed("test_queue_empty");
    }
}

void test_queue_full()
{
    OrderQueue queue;
    init_order_queue(&queue);

    Asset asset;
    asset.market_volume = 1000;

    for (int i = 0; i < MAX_ORDERS; i++)
    {
        sprintf(asset.code, "A%d", i); 
        sprintf(asset.company_name, "Company %d", i);

        Order order;
        order.asset = &asset;
        order.action = BUY;
        order.shares = 100;
        order.price = 50.0;
        order.pending_shares = 100;
        enqueue_order(&queue, &order);
    }

    Asset extra_asset;
    strcpy(extra_asset.code, "A2");
    strcpy(extra_asset.company_name, "Extra Company");
    extra_asset.market_volume = 2000;

    Order extra_order;
    extra_order.asset = &extra_asset;
    extra_order.action = SELL;
    extra_order.shares = 150;
    extra_order.price = 60.0;
    extra_order.pending_shares = 150;
    enqueue_order(&queue, &extra_order);

    if (size(&queue) == MAX_ORDERS)
    {
        test_passed("test_queue_full");
    }
    else
    {
        test_failed("test_queue_full");
    }
}

int main()
{
    log_init("log_test_order_queue.txt");

    test_init_queue();
    test_enqueue_order();
    test_dequeue_order();
    test_queue_empty();
    test_queue_full();

    log_cleanup();

    return 0;
}
