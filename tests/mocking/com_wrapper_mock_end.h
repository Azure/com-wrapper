// Copyright (c) Microsoft. All rights reserved.

/*
This file can be included in unit tests that have mocked COM_WRAPPERs
The corresponding begin header should be included before DEFINE_COM_WRAPPER_OBJECT
This will undo what that file has done
*/

#undef interlocked_exchange
#undef interlocked_increment
#undef interlocked_decrement
