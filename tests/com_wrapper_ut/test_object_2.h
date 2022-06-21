// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef TEST_OBJECT_2_H
#define TEST_OBJECT_2_H

#include "umock_c/umock_c_prod.h"

typedef struct TEST_OBJECT_2_TAG* TEST_OBJECT_2_HANDLE;

MOCKABLE_FUNCTION(, TEST_OBJECT_2_HANDLE, test_object_2_create, const char*, a);
MOCKABLE_FUNCTION(, void, test_object_2_destroy, TEST_OBJECT_2_HANDLE, test_object);
MOCKABLE_FUNCTION(, long, test_object_2_TestMethodWithReturnNoArgs, TEST_OBJECT_2_HANDLE, test_object);
MOCKABLE_FUNCTION(, void, test_object_2_TestMethodWithoutReturnNoArgs, TEST_OBJECT_2_HANDLE, test_object);
MOCKABLE_FUNCTION(, void, test_object_2_TestMethodWithoutReturnWith2Args, TEST_OBJECT_2_HANDLE, test_object, int, a, const char*, b);
MOCKABLE_FUNCTION(, long, test_object_2_TestMethodWithReturnWith2Args, TEST_OBJECT_2_HANDLE, test_object, int, a, const char*, b);
MOCKABLE_FUNCTION(, int, test_object_2_TestMethodInterface2, TEST_OBJECT_2_HANDLE, test_object, int, x);

#endif /* TEST_OBJECT_2_H */
