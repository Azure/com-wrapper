#Copyright (C) Microsoft Corporation. All rights reserved.

# unit tests
if(${run_unittests})
    build_test_folder(com_wrapper_ut)
endif()

# integration tests
if(${run_int_tests})
    build_test_folder(com_wrapper_int)
endif()
