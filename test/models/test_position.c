#include "position.h"

#include <stdio.h>
#include <string.h>

#include "error_codes.h"
#include "log.h"

void test_create_position_valid() {
    Position pos;
    int result = create_position(&pos, "AAPL", 100);
    
    if (result == SUCCESS &&
        strcmp(pos.asset_code, "AAPL") == 0 &&
        pos.shares == 100) {
        printf("test_create_position_valid PASSED\n");
    } else {
        printf("test_create_position_valid FAILED\n");
    }
}

void test_create_position_invalid_asset_code_null() {
    Position pos;
    int result = create_position(&pos, NULL, 100);
    
    if (result == ERR_VALIDATION) {
        printf("test_create_position_invalid_asset_code_null PASSED\n");
    } else {
        printf("test_create_position_invalid_asset_code_null FAILED\n");
    }
}

void test_create_position_invalid_asset_code_too_long() {
    Position pos;
    char long_code[MAX_ASSET_CODE_LENGTH + 1] = "LONGCODE";
    int result = create_position(&pos, long_code, 100);
    
    if (result == ERR_VALIDATION) {
        printf("test_create_position_invalid_asset_code_too_long PASSED\n");
    } else {
        printf("test_create_position_invalid_asset_code_too_long FAILED\n");
    }
}

void test_create_position_negative_shares() {
    Position pos;
    int result = create_position(&pos, "AAPL", -100);
    
    if (result == ERR_VALIDATION) {
        printf("test_create_position_negative_shares PASSED\n");
    } else {
        printf("test_create_position_negative_shares FAILED\n");
    }
}

int main() {
    test_create_position_valid();
    test_create_position_invalid_asset_code_null();
    test_create_position_invalid_asset_code_too_long();
    test_create_position_negative_shares();

    return 0;
}
