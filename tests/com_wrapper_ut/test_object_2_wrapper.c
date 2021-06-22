// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "test_object_2_wrapper.h"
#include "test_object_2.h"
#include "com_wrapper/com_wrapper.h"

/* Tests_SRS_COM_WRAPPER_01_028: [ DEFINE_COM_WRAPPER_OBJECT shall generate all the underlying Vtbl structures needed for the COM object. ]*/
/* Tests_SRS_COM_WRAPPER_01_025: [ The order of the functions shall be the order of the function pointers in the Vtbl structure for implementing_interface. ]*/
/* Tests_SRS_COM_WRAPPER_01_008: [ COM_WRAPPER_IUNKNOWN_APIS implements the QueryInterface, AddRef and Release for implementing_interface for the COM wrapper for the handle type wrapped_handle_type. ]*/
DEFINE_COM_WRAPPER_OBJECT(TEST_OBJECT_2_HANDLE, TEST_OBJECT_2_HANDLE_INTERFACES);
