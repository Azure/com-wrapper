// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "custom_alloc_helpers.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "real_gballoc_hl.h"

static size_t g_custom_malloc_call_count;
static size_t g_custom_free_call_count;

void* com_wrapper_ut_custom_malloc(size_t size)
{
    g_custom_malloc_call_count++;
    return real_gballoc_hl_malloc(size);
}

void com_wrapper_ut_custom_free(void* ptr)
{
    g_custom_free_call_count++;
    real_gballoc_hl_free(ptr);
}

void com_wrapper_ut_reset_custom_alloc_counters(void)
{
    g_custom_malloc_call_count = 0;
    g_custom_free_call_count = 0;
}

size_t com_wrapper_ut_get_custom_malloc_call_count(void)
{
    return g_custom_malloc_call_count;
}

size_t com_wrapper_ut_get_custom_free_call_count(void)
{
    return g_custom_free_call_count;
}
