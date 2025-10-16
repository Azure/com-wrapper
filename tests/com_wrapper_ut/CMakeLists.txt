#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(theseTestsName com_wrapper_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
test_object_2.c
real_test_object.c
test_object_wrapper.c
test_object_2_wrapper.c
test_interface_i.c
)

set(${theseTestsName}_h_files
../../inc/com_wrapper/com_wrapper.h
test_object.h
test_object_wrapper.h
test_object_2_wrapper.h
test_object_2.h
real_test_object.h
real_test_object_renames.h
test_interface.h
)

build_test_artifacts(${theseTestsName} "tests/com_wrapper" ADDITIONAL_LIBS c_pal com_wrapper c_pal_reals
    ENABLE_TEST_FILES_PRECOMPILED_HEADERS "${CMAKE_CURRENT_LIST_DIR}/com_wrapper_ut_pch.h")
