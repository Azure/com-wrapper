// Copyright (c) Microsoft. All rights reserved.

#include <stddef.h>
#include <stdlib.h>

#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "test_object.h"


typedef struct TEST_OBJECT_TAG
{
    char* a;
} TEST_OBJECT;

TEST_OBJECT_HANDLE test_object_create(const char* a)
{
    TEST_OBJECT_HANDLE result = malloc(sizeof(TEST_OBJECT));
    if (result == NULL)
    {
        LogError("malloc failed");
    }
    else
    {
        size_t length = strlen(a) + 1;
        result->a = malloc(length);
        if (result->a == NULL)
        {
            LogError("malloc failed");
        }
        else
        {
            (void)memcpy(result->a, a, length);
            goto all_ok;
        }

        free(result);
    }
    result = NULL;

all_ok:
    return result;
}

void test_object_destroy(TEST_OBJECT_HANDLE test_object)
{
    if (test_object == NULL)
    {
        LogError("Invalid arguments: TEST_OBJECT_HANDLE test_object=%p", test_object);
    }
    else
    {
        free(test_object->a);
        free(test_object);
    }
}

long test_object_TestMethodWithReturnNoArgs(TEST_OBJECT_HANDLE test_object)
{
    (void)test_object;
    return -1;
}

void test_object_TestMethodWithoutReturnNoArgs(TEST_OBJECT_HANDLE test_object)
{
    (void)test_object;
}

void test_object_TestMethodWithoutReturnWith2Args(TEST_OBJECT_HANDLE test_object, int a, const char* b)
{
    (void)test_object;
    (void)a;
    (void)b;
}

long test_object_TestMethodWithReturnWith2Args(TEST_OBJECT_HANDLE test_object, int a, const char* b)
{
    (void)test_object;
    (void)a;
    (void)b;
    return -1;
}
