// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef TEST_OBJECT_2_WRAPPER_H
#define TEST_OBJECT_2_WRAPPER_H

#include "test_object_2.h"
#include "com_wrapper/com_wrapper.h"


#include "Unknwnbase.h"
#include "test_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_OBJECT_2_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(ITestInterface, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(long, test_object_2_TestMethodWithReturnNoArgs), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, test_object_2_TestMethodWithoutReturnNoArgs), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, test_object_2_TestMethodWithoutReturnWith2Args, int, a, const char*, b), \
        COM_WRAPPER_FUNCTION_WRAPPER(long, test_object_2_TestMethodWithReturnWith2Args, int, a, const char*, b) \
    ), \
    COM_WRAPPER_INTERFACE(ITestInterface2, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(int, test_object_2_TestMethodInterface2, int, x) \
    ) \

DECLARE_COM_WRAPPER_OBJECT(TEST_OBJECT_2_HANDLE, TEST_OBJECT_2_HANDLE_INTERFACES);

#ifdef __cplusplus
}
#endif

#endif /* TEST_OBJECT_2_WRAPPER_H */
