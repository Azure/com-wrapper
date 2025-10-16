#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(theseTestsName com_wrapper_int)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
../com_wrapper_ut/test_object.c
custom_alloc_helpers.c
test_object_custom_alloc_wrapper.c
../com_wrapper_ut/test_interface_i.c
)

set(${theseTestsName}_h_files
../../inc/com_wrapper/com_wrapper.h
../com_wrapper_ut/test_object.h
../com_wrapper_ut/test_interface.h
custom_alloc_helpers.h
test_object_custom_alloc_wrapper.h
)

build_test_artifacts(${theseTestsName} "tests/com_wrapper" ADDITIONAL_LIBS c_pal com_wrapper c_pal_reals)
