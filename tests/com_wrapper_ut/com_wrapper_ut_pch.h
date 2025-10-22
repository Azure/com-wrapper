// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Precompiled header for com_wrapper_ut

#ifndef COM_WRAPPER_UT_PCH_H
#define COM_WRAPPER_UT_PCH_H

#include <stdlib.h>
#include <stddef.h>

#include "macro_utils/macro_utils.h"

#include "testrunnerswitcher.h"
#include "umock_c/umock_c.h"
#include "umock_c/umocktypes.h"
#include "umock_c/umocktypes_charptr.h"
#include "c_logging/logger.h"

#include "c_pal/interlocked.h" /*included for mocking reasons - it will prohibit creation of mocks belonging to interlocked.h - at the moment verified through int tests - this is porting legacy code, temporary solution*/

#include "umock_c/umock_c_ENABLE_MOCKS.h" // ============================== ENABLE_MOCKS
#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"
#include "test_object.h"

#include "umock_c/umock_c_DISABLE_MOCKS.h" // ============================== DISABLE_MOCKS

#include "real_gballoc_hl.h"

#include "com_wrapper/com_wrapper.h"
#include "Unknwnbase.h"
#include "test_interface.h"
#include "test_object_2.h"
#include "real_test_object.h"
#include "test_object_wrapper.h"
#include "test_object_2_wrapper.h"

#endif // COM_WRAPPER_UT_PCH_H
