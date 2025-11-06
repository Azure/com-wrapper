// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef TEST_OBJECT_WRAPPER_H
#define TEST_OBJECT_WRAPPER_H

#include "test_object.h"
#include "com_wrapper/com_wrapper.h"


#include "Unknwnbase.h"
#include "windows.h"
#include "test_interface.h"

#define TEST_OBJECT_HANDLE_INTERFACES \
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

/* Tests_SRS_COM_WRAPPER_01_027: [ DECLARE_COM_WRAPPER_OBJECT shall generate constructor prototypes for the COM object for each implemented interface. ]*/
DECLARE_COM_WRAPPER_OBJECT(TEST_OBJECT_HANDLE, TEST_OBJECT_HANDLE_INTERFACES);

#endif /* TEST_OBJECT_WRAPPER_H */
