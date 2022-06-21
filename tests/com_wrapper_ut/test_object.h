// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef TEST_OBJECT_H
#define TEST_OBJECT_H

#include "umock_c/umock_c_prod.h"

typedef struct TEST_OBJECT_TAG* TEST_OBJECT_HANDLE;

MOCKABLE_FUNCTION(, TEST_OBJECT_HANDLE, test_object_create, const char*, a);
MOCKABLE_FUNCTION(, void, test_object_destroy, TEST_OBJECT_HANDLE, test_object);
MOCKABLE_FUNCTION(, long, test_object_TestMethodWithReturnNoArgs, TEST_OBJECT_HANDLE, test_object);
MOCKABLE_FUNCTION(, void, test_object_TestMethodWithoutReturnNoArgs, TEST_OBJECT_HANDLE, test_object);
MOCKABLE_FUNCTION(, void, test_object_TestMethodWithoutReturnWith2Args, TEST_OBJECT_HANDLE, test_object, int, a, const char*, b);
MOCKABLE_FUNCTION(, long, test_object_TestMethodWithReturnWith2Args, TEST_OBJECT_HANDLE, test_object, int, a, const char*, b);

#endif /* TEST_OBJECT_H */
