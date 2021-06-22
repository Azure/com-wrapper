// Copyright (c) Microsoft. All rights reserved.

#ifndef REAL_TEST_OBJECT_H
#define REAL_TEST_OBJECT_H

#include "macro_utils/macro_utils.h"

#define R2(X) REGISTER_GLOBAL_MOCK_HOOK(X, real_##X);

#define REGISTER_TEST_OBJECT_GLOBAL_MOCK_HOOK() \
    MU_FOR_EACH_1(R2, \
        test_object_create, \
        test_object_destroy, \
        test_object_TestMethodWithReturnNoArgs, \
        test_object_TestMethodWithoutReturnNoArgs, \
        test_object_TestMethodWithoutReturnWith2Args, \
        test_object_TestMethodWithReturnWith2Args \
    )

#ifdef __cplusplus
extern "C"
{
#endif

TEST_OBJECT_HANDLE real_test_object_create(const char* a);
void real_test_object_destroy(TEST_OBJECT_HANDLE test_object);
long real_test_object_TestMethodWithReturnNoArgs(TEST_OBJECT_HANDLE test_object);
void real_test_object_TestMethodWithoutReturnNoArgs(TEST_OBJECT_HANDLE test_object);
void real_test_object_TestMethodWithoutReturnWith2Args(TEST_OBJECT_HANDLE test_object, int a, const char* b);
long real_test_object_TestMethodWithReturnWith2Args(TEST_OBJECT_HANDLE test_object, int a, const char* b);

#ifdef __cplusplus
}
#endif

#endif // REAL_TEST_OBJECT_H
