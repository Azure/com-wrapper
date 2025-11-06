`com_wrapper` requirements
================

## Overview

`com_wrapper` is a module that implements wrapping a handle as a COM object with a given set of interfaces.

## Exposed API

```c
#define DECLARE_COM_WRAPPER_OBJECT(wrapped_handle_type, ...) \
    ...

#define DEFINE_COM_WRAPPER_OBJECT(wrapped_handle_type, ...) \
    ...

#define DEFINE_COM_WRAPPER_OBJECT_WITH_MALLOC_FUNCTIONS(wrapped_handle_type, malloc_func, free_func, ...) \
    ...

#define COM_WRAPPER_CREATE(wrapped_handle_type, returned_interface, wrapped_handle, wrapped_handle_destroy) \
    ...

#define COM_WRAPPER_INTERFACE(implementing_interface, ...) \
    ...

#define COM_WRAPPER_IUNKNOWN_APIS() \
    ...

#define COM_WRAPPER_FUNCTION_WRAPPER(return_type, wrapped_function_name, ...)

#define COM_WRAPPER_GET_WRAPPED_HANDLE(wrapped_handle_type, interface_name, COM_object) \
    ...
```

## Example

Let's assume having a handle type `TEST_OBJECT_HANDLE` with the following interface:

```c
typedef struct TEST_OBJECT_TAG* TEST_OBJECT_HANDLE;

TEST_OBJECT_HANDLE test_object_create(const char* a);
void test_object_destroy(TEST_OBJECT_HANDLE test_object);
HRESULT test_object_TestMethod(TEST_OBJECT_HANDLE test_object, int a, const char* b);
```

Let's assume an ITestInterface defined as follows in an `.idl` file:

```c
//=====================================================================================================================
// ITestInterface
//=====================================================================================================================
[
    uuid(...),
    version(1.0),
    local
]
interface ITestInterface : IUnknown
{
    HRESULT TestMethod(int a, const char* b);
};
```

The code generating a COM wrapper implementing the `IUnknown` and `ITestInterface` interfaces looks like:

In the header:

```c
#define COM_WRAPPER_INTERFACES \
    COM_WRAPPER_INTERFACE(IUnknown, \
        COM_WRAPPER_IUNKNOWN_APIS() \
    ), \
    COM_WRAPPER_INTERFACE(ITestInterface, \
        COM_WRAPPER_IUNKNOWN_APIS(), \
        COM_WRAPPER_FUNCTION_WRAPPER(HRESULT, test_object_TestMethod, int, a, const char*, b) \
    ) \

DECLARE_COM_WRAPPER_OBJECT(TEST_OBJECT_HANDLE, COM_WRAPPER_INTERFACES);
```

```c
DEFINE_COM_WRAPPER_OBJECT(TEST_OBJECT_HANDLE, COM_WRAPPER_INTERFACES);
```

Creating an object and releasing it looks like:

```c
    // create the object
    TEST_OBJECT_HANDLE test_object = test_object_create("haga");

    // create the wrapper
    ITestInterface* test_object_ITestInterface = COM_WRAPPER_CREATE(TEST_OBJECT_HANDLE, ITestInterface, test_object, test_object_destroy);

    // release the wrapper
    test_object_ITestInterface->lpVtbl->Release(test_object_ITestInterface);
```

Note: `COM_WRAPPER_CREATE` has `move` semantics, that is, after the interface has been created, the object cannot be released (freed) with _destroy.


### DECLARE_COM_WRAPPER_OBJECT

```c
#define DECLARE_COM_WRAPPER_OBJECT(wrapped_handle_type, ...) \
    ...
```

Arguments:

- `wrapped_handle_type` is the handle wrapped by the COM wrapper object.

- `...` represents a list of interfaces that the COM wrapper implements. Each element in the list has to be constructed with `COM_WRAPPER_INTERFACE`.

**SRS_COM_WRAPPER_01_027: [** `DECLARE_COM_WRAPPER_OBJECT` shall generate constructor prototypes for the COM object for each implemented interface. **]**

### DEFINE_COM_WRAPPER_OBJECT

```c
#define DEFINE_COM_WRAPPER_OBJECT(wrapped_handle_type, ...) \
    ...
```

`DEFINE_COM_WRAPPER_OBJECT` defines a COM wrapper object that wraps a given handle type and implements a set of interfaces.

Arguments:

- `wrapped_handle_type` is the handle wrapped by the COM wrapper object.

- `...` represents a list of interfaces that the COM wrapper implements. Each element in the list has to be constructed with `COM_WRAPPER_INTERFACE`.

**SRS_COM_WRAPPER_01_001: [** `DEFINE_COM_WRAPPER_OBJECT` shall generate constructors for the COM object for each implemented interface. **]**

**SRS_COM_WRAPPER_01_028: [** `DEFINE_COM_WRAPPER_OBJECT` shall generate all the underlying Vtbl structures needed for the COM object. **]**

### DEFINE_COM_WRAPPER_OBJECT_WITH_MALLOC_FUNCTIONS

```c
#define DEFINE_COM_WRAPPER_OBJECT_WITH_MALLOC_FUNCTIONS(wrapped_handle_type, malloc_func, free_func, ...) \
    ...
```

`DEFINE_COM_WRAPPER_OBJECT_WITH_MALLOC_FUNCTIONS` defines a COM wrapper object that wraps a given handle type, implements a set of interfaces, and uses custom allocation functions for the wrapper memory.

Arguments:

- `wrapped_handle_type` is the handle wrapped by the COM wrapper object.

- `malloc_func` is a function used to allocate memory for the COM wrapper object. It shall match the signature `void* (*)(size_t)`.

- `free_func` is a function used to free memory allocated for the COM wrapper object. It shall match the signature `void (*)(void*)`.

- `...` represents a list of interfaces that the COM wrapper implements. Each element in the list has to be constructed with `COM_WRAPPER_INTERFACE`.

**SRS_COM_WRAPPER_66_001: [** `DEFINE_COM_WRAPPER_OBJECT_WITH_MALLOC_FUNCTIONS` shall generate constructors for the COM object for each implemented interface. **]**

**SRS_COM_WRAPPER_66_002: [** `DEFINE_COM_WRAPPER_OBJECT_WITH_MALLOC_FUNCTIONS` shall generate all the underlying Vtbl structures needed for the COM object. **]**

**SRS_COM_WRAPPER_66_003: [** `DEFINE_COM_WRAPPER_OBJECT_WITH_MALLOC_FUNCTIONS` shall use `malloc_func` to allocate memory for the COM wrapper object. **]**

**SRS_COM_WRAPPER_66_004: [** `DEFINE_COM_WRAPPER_OBJECT_WITH_MALLOC_FUNCTIONS` shall use `free_func` to free the memory associated with the COM wrapper object. **]**

### COM_WRAPPER_CREATE

```c
#define COM_WRAPPER_CREATE(wrapped_handle_type, returned_interface, wrapped_handle, wrapped_handle_destroy) \
    ...
```

**SRS_COM_WRAPPER_01_002: [** `COM_WRAPPER_CREATE` shall call the COM object constructor generated by `DEFINE_COM_WRAPPER_OBJECT`. **]**

Note: The `wrapped_handle` instance is transferred to the wrapper with move semantics (if `COM_WRAPPER_CREATE` succeeds, the new COM wrapper owns the handle and has to free it by calling `wrapped_handle_destroy`).

Arguments:

- `wrapped_handle_type` is the handle wrapped by the COM wrapper object.

- `returned_interface` is the interface type for which a pointer shall be returned on success.

- `wrapped_handle` is the handle that is being wrapped.

- `wrapped_handle_destroy` is the function pointer that can be called to dispose of `wrapped_handle`.

**SRS_COM_WRAPPER_01_003: [** `COM_WRAPPER_CREATE` shall allocate enough memory to hold the reference count, the handle `wrapped_handle` and the implemented COM interface Vtbl pointers. **]**

**SRS_COM_WRAPPER_01_004: [** `COM_WRAPPER_CREATE` shall store `wrapped_handle` and `wrapped_handle_destroy` for future use. **]**

**SRS_COM_WRAPPER_01_005: [** `COM_WRAPPER_CREATE` shall initialize the reference count of the COM wrapper to 1. **]**

**SRS_COM_WRAPPER_01_006: [** On success `COM_WRAPPER_CREATE` shall return an interface pointer for `returned_interface`. **]**

**SRS_COM_WRAPPER_01_007: [** If any error occurs, `COM_WRAPPER_CREATE` shall return `NULL`. **]**

### COM_WRAPPER_INTERFACE

```c
#define COM_WRAPPER_INTERFACE(implementing_interface, ...) \
```

`COM_WRAPPER_INTERFACE` allows defining the interface entries to be placed in a Vtbl that for the `implementing_interface` interface.

**SRS_COM_WRAPPER_01_025: [** The order of the functions shall be the order of the function pointers in the Vtbl structure for `implementing_interface`. **]**

**SRS_COM_WRAPPER_01_026: [** `COM_WRAPPER_INTERFACE` shall ensure that enough entries are in `...` to satisfy all functions in the interface. **]**

Each entry in `...` can be one of the following:

#### COM_WRAPPER_IUNKNOWN_APIS

```c
#define COM_WRAPPER_IUNKNOWN_APIS() \
    ...
```

**SRS_COM_WRAPPER_01_008: [** `COM_WRAPPER_IUNKNOWN_APIS` implements the `QueryInterface`, `AddRef` and `Release` for `implementing_interface` for the COM wrapper for the handle type `wrapped_handle_type`. **]**

##### QueryInterface

```c
HRESULT QueryInterface(implementing_interface* This, REFIID riid, _COM_Outptr_ void** ppvObject)
```

**SRS_COM_WRAPPER_01_009: [** If `This` is `NULL`, `QueryInterface` shall fail and return `E_FAIL`. **]**

**SRS_COM_WRAPPER_01_010: [** If `ppvObject` is `NULL`, `QueryInterface` shall fail and return `E_POINTER`. **]**

**SRS_COM_WRAPPER_01_029: [** If `riid` is NULL, `QueryInterface` shall fail and return `E_POINTER`. **]**

**SRS_COM_WRAPPER_01_011: [** If `riid` is one of the interfaces passed in `DEFINE_COM_WRAPPER_OBJECT`, `QueryInterface` shall set `ppvObject` to the interface pointer corresponding to `riid` and return `S_OK`. **]**

**SRS_COM_WRAPPER_01_012: [** If `riid` does not match any of the interfaces passed in `DEFINE_COM_WRAPPER_OBJECT`, `QueryInterface` shall return `E_NOINTERFACE`. **]**

##### AddRef

```c
ULONG AddRef(implementing_interface* This)
```

**SRS_COM_WRAPPER_01_013: [** If `This` is `NULL`, `AddRef` shall fail and return 0. **]**

**SRS_COM_WRAPPER_01_014: [** Otherwise, `AddRef` shall increment the reference count for the object and return the new reference count. **]**

##### Release

```c
ULONG Release(implementing_interface* This)
```

**SRS_COM_WRAPPER_01_015: [** If `This` is `NULL`, `Release` shall fail and return `ULONG_MAX`. **]**

**SRS_COM_WRAPPER_01_016: [** Otherwise, `Release` shall decrement the reference count. **]**

**SRS_COM_WRAPPER_01_017: [** If the reference count reaches 0: **]**

**SRS_COM_WRAPPER_01_018: [** - `Release` shall call the destroy function passed in `COM_WRAPPER_CREATE`. **]**

**SRS_COM_WRAPPER_01_019: [** - `Release` shall free the memory associated with the COM wrapper. **]**

#### COM_WRAPPER_FUNCTION_WRAPPER

```c
#define COM_WRAPPER_FUNCTION_WRAPPER(return_type, wrapped_function_name, ...)
```

Arguments:

- `return_type` is the function return type (HRESULT, void, etc.)

- `wrapped_function_name` is the function name

- `...` is made of pairs of `(arg_type, arg_name)` which represent the arguments of the function.

**SRS_COM_WRAPPER_01_020: [** The macro shall generate a wrapper function that looks like:

```c
return_type STDMETHODCALLTYPE name(implementing_interface* This, arg1_type arg1, arg2_type arg2, ...)
```
 **]**

**SRS_COM_WRAPPER_01_021: [** If `This` is NULL, the wrapper shall return without calling the underlying wrapper. **]**

**SRS_COM_WRAPPER_01_022: [** Otherwise the wrapper shall obtain the handle of the wrapped object. **]**

**SRS_COM_WRAPPER_01_023: [** The wrapper shall call the function `wrapped_function_name`, while passing the first argument to be the object handle and the rest of the arguments passed to the wrapper. **]**

**SRS_COM_WRAPPER_01_024: [** The wrapper shall return the result of `wrapped_function_name`. **]**

### COM_WRAPPER_GET_WRAPPED_HANDLE

```c
#define COM_WRAPPER_GET_WRAPPED_HANDLE(wrapped_handle_type, interface_name, COM_object) \
    ...
```

`COM_WRAPPER_GET_WRAPPED_HANDLE` returns the wrapped handle for the given `COM_object`.

**SRS_COM_WRAPPER_01_030: [** If `COM_object` is `NULL`, `COM_WRAPPER_GET_WRAPPED_HANDLE` shall return `NULL`. **]**

**SRS_COM_WRAPPER_01_031: [** Otherwise, it shall return the handle wrapped by `COM_object`. **]**
