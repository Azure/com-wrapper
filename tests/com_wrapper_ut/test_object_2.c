// Copyright (c) Microsoft. All rights reserved.

#include <stddef.h>
#include <stdlib.h>

#include "c_logging/xlogging.h"
#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "test_object_2.h"

typedef struct TEST_OBJECT_2_TAG
{
    char* a;
} TEST_OBJECT_2;

TEST_OBJECT_2_HANDLE test_object_2_create(const char* a)
{
    TEST_OBJECT_2_HANDLE result = malloc(sizeof(TEST_OBJECT_2));
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

void test_object_2_destroy(TEST_OBJECT_2_HANDLE test_object_2)
{
    if (test_object_2 == NULL)
    {
        LogError("Invalid arguments: TEST_OBJECT_2_HANDLE test_object=%p", test_object_2);
    }
    else
    {
        free(test_object_2->a);
        free(test_object_2);
    }
}

long test_object_2_TestMethodWithReturnNoArgs(TEST_OBJECT_2_HANDLE test_object_2)
{
    (void)test_object_2;
    return -1;
}

void test_object_2_TestMethodWithoutReturnNoArgs(TEST_OBJECT_2_HANDLE test_object_2)
{
    (void)test_object_2;
}

void test_object_2_TestMethodWithoutReturnWith2Args(TEST_OBJECT_2_HANDLE test_object_2, int a, const char* b)
{
    (void)test_object_2;
    (void)a;
    (void)b;
}

long test_object_2_TestMethodWithReturnWith2Args(TEST_OBJECT_2_HANDLE test_object_2, int a, const char* b)
{
    (void)test_object_2;
    (void)a;
    (void)b;
    return -1;
}

int test_object_2_TestMethodInterface2(TEST_OBJECT_2_HANDLE test_object_2, int x)
{
    (void)test_object_2;
    (void)x;
    return 0;
}
