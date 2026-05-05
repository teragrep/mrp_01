#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <mysql/mysql.h>
#include <sodium.h>
#include <string.h>

#include "../lib_sodium_ristretto.h"

void testPassRistrettoisvalidpoint_init() {
    char *RistrettoPoint = malloc (32);
    size_t i;
    for (i = 0; i < 32; i++)
    {
        RistrettoPoint[i] = rand ();
    }
    char * testArgs[] = {RistrettoPoint};
    unsigned long values[1] = {strlen(testArgs[0])};
    unsigned long * testLengths = values;
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null=0};
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = 21, .ptr = "0", .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoisvalidpoint_init(&initid, &args, message);
    assert(result == 0 && "Result is not 0, _init failed when it should have passed.");
    printf("testPassRistrettoisvalidpoint_init() passed assertions!\n");
    free(RistrettoPoint);
}

void testInvalidArgSizeRistrettoisvalidpoint_init() {
    char *RistrettoPoint = malloc (64);
    size_t i;
    for (i = 0; i < 64; i++)
    {
        RistrettoPoint[i] = rand ();
    }
    char * testArgs[] = {RistrettoPoint};
    unsigned long values[1] = {strlen(testArgs[0])};
    unsigned long * testLengths = values;
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 1, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null=0};
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = 21, .ptr = "0", .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoisvalidpoint_init(&initid, &args, message);
    assert(result == 1 && "Result is not 1, _init passed when it should have failed.");
    assert(strcmp(message, "First input argument is not a 32 byte binary string") == 0 && "Error message is incorrect");
    printf("testInvalidArgSizeRistrettoisvalidpoint_init() passed assertions!\n");
}

void testInvalidArgAmountRistrettoisvalidpoint_init() {
    char *RistrettoPoint = malloc (32);
    size_t i;
    for (i = 0; i < 32; i++)
    {
        RistrettoPoint[i] = rand ();
    }
    char * testArgs[] = {RistrettoPoint, RistrettoPoint};
    unsigned long values[2] = {strlen(testArgs[0]), strlen(testArgs[1])};
    unsigned long * testLengths = values;
    enum Item_result itemValue[1] = {STRING_RESULT};
    UDF_ARGS args = { .arg_count = 2, .arg_type = itemValue, .args = testArgs, .lengths = testLengths, .maybe_null=0};
    UDF_INIT initid = {.maybe_null=0, .decimals = 3, .max_length = 21, .ptr = "0", .const_item = 0};
    char message[MYSQL_ERRMSG_SIZE];
    my_bool result = ristrettoisvalidpoint_init(&initid, &args, message);
    assert(result == 1 && "Result is not 1, _init passed when it should have failed.");
    assert(strcmp(message,"requires 1 binary string argument") == 0 && "Error message is incorrect");
    printf("testInvalidArgAmountRistrettoisvalidpoint_init() passed assertions!\n");
    free(RistrettoPoint);
}

int main()
{
    testPassRistrettoisvalidpoint_init();
    testInvalidArgSizeRistrettoisvalidpoint_init();
    testInvalidArgAmountRistrettoisvalidpoint_init();
    return 0;
}