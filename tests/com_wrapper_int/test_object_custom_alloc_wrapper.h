// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TEST_OBJECT_CUSTOM_ALLOC_WRAPPER_H
#define TEST_OBJECT_CUSTOM_ALLOC_WRAPPER_H

#include "test_object.h"
#include "com_wrapper/com_wrapper.h"

#include "Unknwnbase.h"
#include "windows.h"
#include "test_interface.h"

typedef TEST_OBJECT_HANDLE TEST_OBJECT_CUSTOM_ALLOC_HANDLE;

#define TEST_OBJECT_CUSTOM_ALLOC_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(ITestInterface, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(long, test_object_TestMethodWithReturnNoArgs), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, test_object_TestMethodWithoutReturnNoArgs), \
        COM_WRAPPER_FUNCTION_WRAPPER(void, test_object_TestMethodWithoutReturnWith2Args, int, a, const char*, b), \
        COM_WRAPPER_FUNCTION_WRAPPER(long, test_object_TestMethodWithReturnWith2Args, int, a, const char*, b) \
    )

DECLARE_COM_WRAPPER_OBJECT(TEST_OBJECT_CUSTOM_ALLOC_HANDLE, TEST_OBJECT_CUSTOM_ALLOC_HANDLE_INTERFACES);

#endif /* TEST_OBJECT_CUSTOM_ALLOC_WRAPPER_H */
