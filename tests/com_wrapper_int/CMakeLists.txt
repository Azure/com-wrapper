#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(theseTestsName com_wrapper_int)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
    test_object.c
    test_object_custom_alloc_wrapper.c
    test_interface_i.c
)

set(${theseTestsName}_h_files
    ../../inc/com_wrapper/com_wrapper.h
    test_object.h
    test_interface.h
    test_object_custom_alloc_wrapper.h
)

build_test_artifacts(${theseTestsName} "tests/com_wrapper" ADDITIONAL_LIBS c_pal com_wrapper c_pal_reals)
