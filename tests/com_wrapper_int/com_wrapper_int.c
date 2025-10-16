// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "testrunnerswitcher.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "real_gballoc_hl.h"

#include "test_object_custom_alloc_wrapper.h"

BEGIN_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)

TEST_SUITE_INITIALIZE(suite_initialize)
{
    ASSERT_ARE_EQUAL(int, 0, real_gballoc_hl_init(NULL, NULL));
}

TEST_SUITE_CLEANUP(suite_cleanup)
{
    real_gballoc_hl_deinit();
}

TEST_FUNCTION_INITIALIZE(test_initialize)
{
}

TEST_FUNCTION_CLEANUP(test_cleanup)
{
}

TEST_FUNCTION(custom_allocator_create_and_release_succeeds)
{
    // arrange
    TEST_OBJECT_HANDLE test_object = test_object_create("haga");
    ASSERT_IS_NOT_NULL(test_object);

    // act
    ITestInterface* custom_interface = COM_WRAPPER_CREATE(TEST_OBJECT_CUSTOM_ALLOC_HANDLE, ITestInterface, (TEST_OBJECT_CUSTOM_ALLOC_HANDLE)test_object, test_object_destroy);

    // assert
    ASSERT_IS_NOT_NULL(custom_interface);

    // cleanup
    (void)custom_interface->lpVtbl->Release(custom_interface);
}

END_TEST_SUITE(TEST_SUITE_NAME_FROM_CMAKE)
