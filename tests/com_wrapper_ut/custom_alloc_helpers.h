// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef CUSTOM_ALLOC_HELPERS_H
#define CUSTOM_ALLOC_HELPERS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* com_wrapper_ut_custom_malloc(size_t size);
void com_wrapper_ut_custom_free(void* ptr);

void com_wrapper_ut_reset_custom_alloc_counters(void);
size_t com_wrapper_ut_get_custom_malloc_call_count(void);
size_t com_wrapper_ut_get_custom_free_call_count(void);

#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_ALLOC_HELPERS_H */
