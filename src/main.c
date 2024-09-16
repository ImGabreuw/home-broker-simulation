#include <stdio.h>

#include "log.h"

int main(int argc, char const *argv[])
{
    char log_filename[] = "home_broker_log.txt";
    log_init(log_filename);

    log_message(LOG_INFO, "Home Breaker started...");

    log_cleanup();

    return 0;
}