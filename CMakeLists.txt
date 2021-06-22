#Copyright (C) Microsoft Corporation. All rights reserved.

include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)

FILE(GLOB com_wrapper_md_files "devdoc/*.md")
SOURCE_GROUP(devdoc FILES ${com_wrapper_md_files})

add_library(com_wrapper INTERFACE)
target_include_directories(com_wrapper INTERFACE ${CMAKE_CURRENT_LIST_DIR}/inc)

add_subdirectory(tests)

