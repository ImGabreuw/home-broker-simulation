#include "scheduler.h"

#include "investor_queue.h"
#include "investor.h"
#include "log.h"
#include "error_codes.h"
#include "test_utils.h"

void test_create_investor_valid()
{
    Investor investor;
    pthread_t thread_id = pthread_self();
    int result = create_investor(&investor, thread_id, "Valid Investor");

    if (result == SUCCESS)
    {
        test_passed("test_create_investor_valid");
    }
    else
    {
        test_failed("test_create_investor_valid");
    }
}

void test_create_investor_invalid_name()
{
    Investor investor;
    pthread_t thread_id = pthread_self();
    int result = create_investor(&investor, thread_id, "This name is definitely too long for the maximum allowed length");

    if (result == ERR_VALIDATION)
    {
        test_passed("test_create_investor_invalid_name");
    }
    else
    {
        test_failed("test_create_investor_invalid_name");
    }
}

void test_enqueue_investor()
{
    InvestorQueue queue;
    Investor investor;
    pthread_t thread_id = pthread_self();
    int result = create_investor(&investor, thread_id, "Investor for Queue Test");

    init_queue(&queue);
    enqueue_investor(&queue, &investor);

    if (queue.count == 1)
    {
        test_passed("test_enqueue_investor");
    }
    else
    {
        test_failed("test_enqueue_investor");
    }
}

void test_dequeue_investor()
{
    InvestorQueue queue;
    Investor investor1, investor2;
    pthread_t thread_id1 = pthread_self();
    pthread_t thread_id2 = pthread_self();

    create_investor(&investor1, thread_id1, "Investor 1");
    create_investor(&investor2, thread_id2, "Investor 2");

    init_queue(&queue);
    enqueue_investor(&queue, &investor1);
    enqueue_investor(&queue, &investor2);

    Investor *dequeued_investor = dequeue_investor(&queue);

    if (dequeued_investor == &investor1 && queue.count == 1)
    {
        test_passed("test_dequeue_investor");
    }
    else
    {
        test_failed("test_dequeue_investor");
    }
}

void test_investor_lifecycle()
{
    Investor investor;
    pthread_t thread_id = pthread_self();
    create_investor(&investor, thread_id, "Investor Lifecycle Test");

    pthread_t investor_thread;
    int result = pthread_create(&investor_thread, NULL, investor_lifecycle, (void *)&investor);

    if (result == 0)
    {
        pthread_join(investor_thread, NULL);
        test_passed("test_investor_lifecycle");
    }
    else
    {
        test_failed("test_investor_lifecycle");
    }
}

void test_scheduler()
{
    Investor investors[MAX_INVESTORS];
    pthread_t investor_threads[MAX_INVESTORS];

    for (int i = 0; i < MAX_INVESTORS; i++)
    {
        char name[50];
        snprintf(name, 50, "Investor %d", i + 1);
        create_investor(&investors[i], investor_threads[i], name);
    }

    simulate_investor_scheduling(investors, MAX_INVESTORS);
}

int main()
{
    log_init("log_scheduler_test.txt");

    test_create_investor_valid();
    test_create_investor_invalid_name();
    test_enqueue_investor();
    test_dequeue_investor();
    test_investor_lifecycle();
    test_scheduler();

    log_cleanup();
    return 0;
}
