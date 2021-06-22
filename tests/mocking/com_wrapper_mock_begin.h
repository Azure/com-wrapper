// Copyright (c) Microsoft. All rights reserved.

/*
This file can be included in unit tests that have mocked COM_WRAPPERs
It will do renames to reals for any internal calls used by the wrapper so they don't show up as mocked calls
The corresponding end header should be included after all of the DEFINE_COM_WRAPPER_OBJECT calls
*/

#include "real_interlocked.h"

#define interlocked_exchange real_interlocked_exchange
#define interlocked_increment real_interlocked_increment
#define interlocked_decrement real_interlocked_decrement
