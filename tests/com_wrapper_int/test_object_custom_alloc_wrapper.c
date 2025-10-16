// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "test_object_custom_alloc_wrapper.h"

#include <stdlib.h>
#include "test_object.h"

DEFINE_COM_WRAPPER_OBJECT_WITH_MALLOC_FUNCTIONS(TEST_OBJECT_CUSTOM_ALLOC_HANDLE, malloc, free, TEST_OBJECT_CUSTOM_ALLOC_HANDLE_INTERFACES);
