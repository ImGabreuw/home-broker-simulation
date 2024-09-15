#include "investor.h"

#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "log.h"
#include "error_codes.h"
#include "test_utils.h"

void test_create_investor()
{
    Investor investor;
    pthread_t id = pthread_self();
    int result = create_investor(&investor, id, "John Doe");

    if (result == SUCCESS && strcmp(investor.name, "John Doe") == 0)
    {
        test_passed("test_create_investor");
    }
    else
    {
        test_failed("test_create_investor");
    }
}

void test_add_asset_position()
{
    Investor investor;
    pthread_t id = pthread_self();
    create_investor(&investor, id, "John Doe");

    Position position = {"A1234", 100};
    int result = add_asset_position(&investor, &position);

    Position *pos = get_asset_position(&investor, "A1234");
    if (result == SUCCESS && pos != NULL && pos->shares == 100)
    {
        test_passed("test_add_asset_position");
    }
    else
    {
        test_failed("test_add_asset_position");
    }
}

void test_update_asset_position()
{
    Investor investor;
    pthread_t id = pthread_self();
    create_investor(&investor, id, "John Doe");

    Position position = {"A1234", 100};
    add_asset_position(&investor, &position);
    int result = update_asset_position(&investor, "A1234", 200);

    Position *pos = get_asset_position(&investor, "A1234");
    if (result == SUCCESS && pos != NULL && pos->shares == 200)
    {
        test_passed("test_update_asset_position");
    }
    else
    {
        test_failed("test_update_asset_position");
    }
}

void test_get_asset_position()
{
    Investor investor;
    pthread_t id = pthread_self();
    create_investor(&investor, id, "John Doe");

    Position position = {"A1234", 100};
    add_asset_position(&investor, &position);

    Position *pos = get_asset_position(&investor, "A1234");
    if (pos != NULL)
    {
        test_passed("test_get_asset_position");
    }
    else
    {
        test_failed("test_get_asset_position");
    }
}

int main()
{
    test_create_investor();
    test_add_asset_position();
    test_update_asset_position();
    test_get_asset_position();

    return 0;
}
