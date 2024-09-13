#include "error_codes.h"

#include <stdio.h>
#include <stdlib.h>

const char *get_error_message(int error_code)
{
    switch (error_code)
    {
    case ERR_UNCHECKED:
        return "Unchecked error occurred.";
    case ERR_VALIDATION:
        return "Validation error.";
    case ERR_MEMORY_ALLOCATION:
        return "Memory allocation error.";
    case ERR_FILE_NOT_FOUND:
        return "File not found.";
    case SUCCESS:
        return "Operation completed successfully.";
    default:
        return "Unknown error.";
    }
}

bool is_error(int result)
{
    return result < 0;
}
