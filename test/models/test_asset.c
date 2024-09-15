#include "asset.h"

#include <stdio.h>
#include <string.h>

#include "log.h"
#include "error_codes.h"
#include "test_utils.h"

void test_create_asset_valid() {
    Asset asset;
    int result = create_asset(&asset, "A1234", "Example Company", 10000);

    if (result == SUCCESS) {
        test_passed("test_create_asset_valid");
    } else {
        test_failed("test_create_asset_valid");
    }
}

void test_create_asset_invalid_code() {
    Asset asset;
    int result = create_asset(&asset, "A123456", "Example Company", 10000);

    if (result == ERR_VALIDATION) {
        test_passed("test_create_asset_invalid_code");
    } else {
        test_failed("test_create_asset_invalid_code");
    }
}

void test_create_asset_invalid_company_name() {
    Asset asset;
    int result = create_asset(&asset, "A1234", "This company name is way too long to be valid.  This company name is way too long to be valid.", 10000);

    if (result == ERR_VALIDATION) {
        test_passed("test_create_asset_invalid_company_name");
    } else {
        test_failed("test_create_asset_invalid_company_name");
    }
}

void test_create_asset_negative_market_volume() {
    Asset asset;
    int result = create_asset(&asset, "A1234", "Example Company", -10000);

    if (result == ERR_VALIDATION) {
        test_passed("test_create_asset_negative_market_volume");
    } else {
        test_failed("test_create_asset_negative_market_volume");
    }
}

int main() {
    test_create_asset_valid();
    test_create_asset_invalid_code();
    test_create_asset_invalid_company_name();
    test_create_asset_negative_market_volume();

    return 0;
}
