# com-wrapper AI Coding Instructions

## Project Overview
com-wrapper is a C library that provides a COM (Component Object Model) wrapper framework for binding COM interfaces to C pseudo-object handles. It enables wrapping C handle-based objects with COM interface implementations, allowing seamless integration between C code and COM-based systems.

## External Dependencies and Standards
All code must follow the comprehensive coding standards defined in #file:../deps/c-build-tools/.github/general_coding_instructions.md. For detailed patterns and conventions, refer to dependency-specific instructions:

- **Build Infrastructure**: #file:../deps/c-build-tools/.github/copilot-instructions.md
- **Macro Utilities**: #file:../deps/macro-utils-c/.github/copilot-instructions.md
- **Logging Framework**: #file:../deps/c-logging/.github/copilot-instructions.md
- **Platform Abstraction**: #file:../deps/c-pal/.github/copilot-instructions.md
- **Test Framework**: #file:../deps/ctest/.github/copilot-instructions.md
- **Test Runner**: #file:../deps/c-testrunnerswitcher/.github/copilot-instructions.md
- **Mocking Framework**: #file:../deps/umock-c/.github/copilot-instructions.md

## Architecture & Core Concepts

### COM Wrapper System
com-wrapper uses advanced macro metaprogramming to generate COM objects that wrap C handles:

- **Handle Wrapping**: Any C handle type can be wrapped with COM interfaces
- **Interface-Based Design**: Supports multiple COM interfaces per wrapped object (IUnknown + custom interfaces)
- **Reference Counting**: Automatic COM reference counting with proper object lifecycle management
- **Move Semantics**: `COM_WRAPPER_CREATE` transfers handle ownership to the COM wrapper

### Key Macro Components

#### Object Declaration and Definition
```c
// 1. Define interface specification macro
#define MY_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(IMyInterface, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, my_object_method, int, param1, const char*, param2) \
    )

// 2. Declare COM wrapper in header
DECLARE_COM_WRAPPER_OBJECT(MY_HANDLE, MY_HANDLE_INTERFACES);

// 3. Define COM wrapper implementation in source file
DEFINE_COM_WRAPPER_OBJECT(MY_HANDLE, MY_HANDLE_INTERFACES);
```

#### COM Object Creation and Usage
```c
// Create the underlying C handle
MY_HANDLE my_handle = my_object_create("parameter");

// Wrap with COM interface (handle ownership transfers to COM wrapper)
IMyInterface* com_interface = COM_WRAPPER_CREATE(MY_HANDLE, IMyInterface, my_handle, my_object_destroy);

// Use COM interface normally
HRESULT hr = com_interface->lpVtbl->MyMethod(com_interface, 42, "test");

// COM reference counting manages lifecycle
(void)com_interface->lpVtbl->Release(com_interface);  // Calls my_object_destroy when ref count reaches 0
```

#### Interface Definition Patterns
```c
// Interface with multiple methods
COM_WRAPPER_INTERFACE(ITestInterface,
    COM_WRAPPER_IUNKNOWN_APIS(),                                              // QueryInterface, AddRef, Release
    COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, handle_method_with_return, int, a, const char*, b),
    COM_WRAPPER_FUNCTION_WRAPPER(void, handle_method_without_return, int, param),
    COM_WRAPPER_FUNCTION_WRAPPER(long, handle_method_complex, BUFFER*, buffer, size_t, size)
)

// Multiple interfaces on same handle
#define MULTI_INTERFACE_SPEC \
    COM_WRAPPER_INTERFACE(IUnknown, COM_WRAPPER_IUNKNOWN_APIS()), \
    COM_WRAPPER_INTERFACE(IInterface1, COM_WRAPPER_IUNKNOWN_APIS(), /* methods */), \
    COM_WRAPPER_INTERFACE(IInterface2, COM_WRAPPER_IUNKNOWN_APIS(), /* methods */)
```

### Generated Code Structure
The macros generate:
- **COM wrapper struct**: Contains interface vtables, handle, destroy function, reference count
- **Vtable implementations**: Function pointers for each interface method
- **Constructor functions**: Type-specific creators for each interface
- **QueryInterface support**: Interface lookup and reference counting
- **Handle extraction**: `COM_WRAPPER_GET_WRAPPED_HANDLE` for accessing original handle

## Testing Patterns with umock-c

### Unit Test Structure
```c
#include "testrunnerswitcher.h"
#include "c_pal/interlocked.h"          // Typically not mocked

#define ENABLE_MOCKS
#include "c_pal/gballoc_hl.h"           // Mock memory allocation
#include "test_object.h"                // Mock the wrapped handle functions
#undef ENABLE_MOCKS

#include "real_gballoc_hl.h"
#include "com_wrapper/com_wrapper.h"
#include "test_object_wrapper.h"        // COM wrapper under test

// Test COM object creation and method calls
TEST_FUNCTION(com_wrapper_create_and_call_succeeds)
{
    // arrange
    TEST_OBJECT_HANDLE mock_handle = (TEST_OBJECT_HANDLE)0x1234;
    
    STRICT_EXPECTED_CALL(test_object_create("test_param"))
        .SetReturn(mock_handle);
    STRICT_EXPECTED_CALL(malloc(IGNORED_ARG))
        .SetReturn((void*)0x5678);
    
    // act
    ITestInterface* interface = COM_WRAPPER_CREATE(TEST_OBJECT_HANDLE, ITestInterface, 
                                                  test_object_create("test_param"), test_object_destroy);
    
    // assert
    ASSERT_IS_NOT_NULL(interface);
    ASSERT_ARE_EQUAL(char_ptr, umock_c_get_expected_calls(), umock_c_get_actual_calls());
    
    // cleanup
    interface->lpVtbl->Release(interface);
}
```

### Reference Counting Verification
```c
// Verify proper reference counting behavior
TEST_FUNCTION(com_wrapper_queryinterface_increments_ref_count)
{
    // arrange
    ITestInterface* interface1 = create_test_interface();
    IUnknown* unknown = NULL;
    
    // act
    HRESULT hr = interface1->lpVtbl->QueryInterface(interface1, &IID_IUnknown, (void**)&unknown);
    
    // assert
    ASSERT_ARE_EQUAL(int, S_OK, hr);
    ASSERT_IS_NOT_NULL(unknown);
    
    // Both interfaces should point to same underlying object
    TEST_OBJECT_HANDLE handle1 = COM_WRAPPER_GET_WRAPPED_HANDLE(TEST_OBJECT_HANDLE, ITestInterface, interface1);
    TEST_OBJECT_HANDLE handle2 = COM_WRAPPER_GET_WRAPPED_HANDLE(TEST_OBJECT_HANDLE, IUnknown, unknown);
    ASSERT_ARE_EQUAL(void_ptr, handle1, handle2);
    
    // cleanup - both references must be released
    interface1->lpVtbl->Release(interface1);
    unknown->lpVtbl->Release(unknown);
}
```

### Error Handling Verification
```c
// Test parameter validation and error conditions
TEST_FUNCTION(com_wrapper_create_with_null_handle_returns_null)
{
    // arrange
    // act
    ITestInterface* interface = COM_WRAPPER_CREATE(TEST_OBJECT_HANDLE, ITestInterface, NULL, test_object_destroy);
    
    // assert
    ASSERT_IS_NULL(interface);
}

TEST_FUNCTION(queryinterface_with_unsupported_iid_returns_no_interface)
{
    // arrange
    ITestInterface* interface = create_test_interface();
    void* result = NULL;
    
    // act
    HRESULT hr = interface->lpVtbl->QueryInterface(interface, &IID_ITestInterfaceNotImpl, &result);
    
    // assert
    ASSERT_ARE_EQUAL(int, E_NOINTERFACE, hr);
    ASSERT_IS_NULL(result);
    
    // cleanup
    interface->lpVtbl->Release(interface);
}
```

## Project-Specific Conventions

### Naming Conventions
- **Wrapper Macros**: `{HANDLE_TYPE}_INTERFACES` for interface specifications
- **Generated Functions**: `COM_WRAPPER_TYPE_CREATE_{HANDLE_TYPE}_{INTERFACE_TYPE}`
- **Handle Extraction**: `COM_WRAPPER_TYPE_GET_WRAPPED_HANDLE_{HANDLE_TYPE}_{INTERFACE_TYPE}`

### Error Handling Patterns
- **Creation Failure**: `COM_WRAPPER_CREATE` returns `NULL` on any error
- **Parameter Validation**: All COM methods validate `This` pointer, return appropriate HRESULT
- **Memory Management**: COM wrapper owns the wrapped handle after successful creation

### Integration with IDL-Generated Interfaces
```c
// Generated from IDL file
interface ITestInterface : IUnknown
{
    HRESULT TestMethod(int a, const char* b);
    HRESULT AnotherMethod(void);
};

// Corresponding COM wrapper specification
COM_WRAPPER_INTERFACE(ITestInterface,
    COM_WRAPPER_IUNKNOWN_APIS(),
    COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, my_handle_test_method, int, a, const char*, b),
    COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, my_handle_another_method)
)
```

## Advanced Features

### Multiple Interface Support
```c
// Single object implementing multiple interfaces
THANDLE(MY_OBJECT) obj = create_my_object();

// Get different interface pointers to same object
IInterface1* if1 = COM_WRAPPER_CREATE(MY_OBJECT_HANDLE, IInterface1, obj, my_object_destroy);
IInterface2* if2 = NULL;

// QueryInterface for second interface
HRESULT hr = if1->lpVtbl->QueryInterface(if1, &IID_IInterface2, (void**)&if2);

// Both interfaces control same underlying object
MY_OBJECT_HANDLE handle1 = COM_WRAPPER_GET_WRAPPED_HANDLE(MY_OBJECT_HANDLE, IInterface1, if1);
MY_OBJECT_HANDLE handle2 = COM_WRAPPER_GET_WRAPPED_HANDLE(MY_OBJECT_HANDLE, IInterface2, if2);
// handle1 == handle2
```

### Custom Type Validation
```c
// Runtime validation of interface compatibility
#define COM_WRAPPER_VTBL_CHECK(handle_type, interface_type) \
    static_assert(sizeof(interface_type##Vtbl) == sizeof(handle_type##_##interface_type##_Vtbl_CHECK_STRUCT), \
                 "Interface method count mismatch")
```

### Function Wrapper Variants
```c
// Different return types and parameter patterns
COM_WRAPPER_FUNCTION_WRAPPER(void, handle_void_method, int, param),              // void return
COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, handle_hresult_method, LPCSTR, input),     // HRESULT return
COM_WRAPPER_FUNCTION_WRAPPER(long, handle_long_method),                          // no parameters
COM_WRAPPER_FUNCTION_WRAPPER(int, handle_complex_method, void*, buffer, SIZE_T, size, DWORD*, result)  // multiple params
```

## External Dependencies & Standards

### Dependency References
For comprehensive patterns, refer to dependency-specific instructions:
- **c-build-tools**: Build infrastructure and coding standards (#file:../deps/c-build-tools/.github/copilot-instructions.md)
- **macro-utils-c**: Advanced macro metaprogramming techniques (#file:../deps/macro-utils-c/.github/copilot-instructions.md)
- **umock-c**: C mocking framework for COM interface testing (#file:../deps/umock-c/.github/copilot-instructions.md)
- **c-pal**: Platform abstraction and memory management (#file:../deps/c-pal/.github/copilot-instructions.md)
- **c-logging**: Structured logging for debugging and tracing (#file:../deps/c-logging/.github/copilot-instructions.md)

### Coding Standards Compliance
**CRITICAL**: All code changes must follow comprehensive standards in #file:../deps/c-build-tools/.github/general_coding_instructions.md:
- **Function naming**: snake_case with `com_wrapper_` prefix for internal functions
- **Parameter validation**: COM methods validate parameters and return appropriate HRESULTs
- **Header inclusion order**: Standard C headers, Windows COM headers, infrastructure headers, project headers
- **Requirements traceability**: `SRS_COM_WRAPPER_##_###` -> `Codes_SRS_COM_WRAPPER_##_###` -> `Tests_SRS_COM_WRAPPER_##_###`
- **Memory management**: Include `c_pal/gballoc_hl_redirect.h` in all source files
- **Error handling**: Consistent goto cleanup patterns and single exit points

## CI/CD and Build Pipeline
- **Azure DevOps**: Uses `c_build_tools` pipeline templates for automated builds
- **Multi-Allocator Testing**: Tests with PASSTHROUGH and JEMALLOC memory allocators
- **Security Scanning**: CodeQL3000 integration for static analysis
- **Cross-Platform**: Windows primary, with Linux compatibility through c-pal

## Key Files & Components
- **`inc/com_wrapper/com_wrapper.h`**: Core macro definitions and COM wrapper framework
- **`devdoc/com_wrapper_requirements.md`**: Comprehensive requirements specification with SRS traceability
- **`tests/com_wrapper_ut/`**: Unit tests demonstrating COM wrapper usage patterns
- **`tests/com_wrapper_ut/test_object_wrapper.*`**: Example COM wrapper implementation
- **`tests/com_wrapper_ut/test_interface.idl`**: Sample IDL interface for testing

This framework enables C libraries to expose COM-compatible interfaces while maintaining C handle-based object semantics and automatic memory management through COM reference counting.

## Common Patterns and Anti-Patterns

### Recommended Patterns
```c
// [GOOD] Proper interface specification with consistent naming
#define MY_HANDLE_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, COM_WRAPPER_IUNKNOWN_APIS()), \
    COM_WRAPPER_INTERFACE(IMyInterface, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, my_handle_operation, int, param) \
    )

// [GOOD] Handle creation with proper error checking
MY_HANDLE handle = my_object_create(params);
if (handle == NULL) { /* handle error */ }

IMyInterface* com_obj = COM_WRAPPER_CREATE(MY_HANDLE, IMyInterface, handle, my_object_destroy);
if (com_obj == NULL) { 
    my_object_destroy(handle);  // COM wrapper creation failed, clean up handle
    /* handle error */
}

// [GOOD] Proper COM reference counting
com_obj->lpVtbl->AddRef(com_obj);          // Increment when storing reference
/* use object */
com_obj->lpVtbl->Release(com_obj);         // Decrement when done
```

### Critical Mistakes to Avoid
```c
// [BAD] Wrong function signature in wrapper
// IDL: HRESULT TestMethod(int param);
// C function: int my_object_test(MY_HANDLE handle, int param);
COM_WRAPPER_FUNCTION_WRAPPER(int, my_object_test, int, param);  // WRONG - return type mismatch

// [GOOD] Correct function signature matching
COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, my_object_test, int, param);  // Must return HRESULT

// [BAD] Using handle after COM wrapper creation (handle ownership transferred)
MY_HANDLE handle = my_object_create(params);
IMyInterface* com_obj = COM_WRAPPER_CREATE(MY_HANDLE, IMyInterface, handle, my_object_destroy);
my_object_operation(handle);  // WRONG - handle owned by COM wrapper now

// [GOOD] Access handle through COM wrapper when needed
MY_HANDLE retrieved = COM_WRAPPER_GET_WRAPPED_HANDLE(MY_HANDLE, IMyInterface, com_obj);
my_object_operation(retrieved);  // Correct - but be careful about lifetime

// [BAD] Manual cleanup after COM wrapper creation
MY_HANDLE handle = my_object_create(params);
IMyInterface* com_obj = COM_WRAPPER_CREATE(MY_HANDLE, IMyInterface, handle, my_object_destroy);
my_object_destroy(handle);  // WRONG - double-free, COM wrapper will destroy handle

// [GOOD] Let COM reference counting manage cleanup
IMyInterface* com_obj = COM_WRAPPER_CREATE(MY_HANDLE, IMyInterface, handle, my_object_destroy);
/* use com_obj */
com_obj->lpVtbl->Release(com_obj);  // This will call my_object_destroy when ref count reaches 0
```
