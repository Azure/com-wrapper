

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for test_interface.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __test_interface_h__
#define __test_interface_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITestInterface_FWD_DEFINED__
#define __ITestInterface_FWD_DEFINED__
typedef interface ITestInterface ITestInterface;

#endif 	/* __ITestInterface_FWD_DEFINED__ */


#ifndef __ITestInterface2_FWD_DEFINED__
#define __ITestInterface2_FWD_DEFINED__
typedef interface ITestInterface2 ITestInterface2;

#endif 	/* __ITestInterface2_FWD_DEFINED__ */


#ifndef __ITestInterfaceNotImpl_FWD_DEFINED__
#define __ITestInterfaceNotImpl_FWD_DEFINED__
typedef interface ITestInterfaceNotImpl ITestInterfaceNotImpl;

#endif 	/* __ITestInterfaceNotImpl_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_test_interface_0000_0000 */
/* [local] */ 






extern RPC_IF_HANDLE __MIDL_itf_test_interface_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_test_interface_0000_0000_v0_0_s_ifspec;

#ifndef __ITestInterface_INTERFACE_DEFINED__
#define __ITestInterface_INTERFACE_DEFINED__

/* interface ITestInterface */
/* [object][local][version][uuid] */ 


EXTERN_C const IID IID_ITestInterface;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("42CAEA39-1A18-4199-B537-D9EF61C26D6E")
    ITestInterface : public IUnknown
    {
    public:
        virtual long STDMETHODCALLTYPE TestMethodWithReturnNoArgs( void) = 0;
        
        virtual void STDMETHODCALLTYPE TestMethodWithoutReturnNoArgs( void) = 0;
        
        virtual void STDMETHODCALLTYPE TestMethodWithoutReturnWith2Args( 
            int a,
            LPCSTR b) = 0;
        
        virtual long STDMETHODCALLTYPE TestMethodWithReturnWith2Args( 
            int a,
            LPCSTR b) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITestInterfaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITestInterface * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITestInterface * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITestInterface * This);
        
        long ( STDMETHODCALLTYPE *TestMethodWithReturnNoArgs )( 
            ITestInterface * This);
        
        void ( STDMETHODCALLTYPE *TestMethodWithoutReturnNoArgs )( 
            ITestInterface * This);
        
        void ( STDMETHODCALLTYPE *TestMethodWithoutReturnWith2Args )( 
            ITestInterface * This,
            int a,
            LPCSTR b);
        
        long ( STDMETHODCALLTYPE *TestMethodWithReturnWith2Args )( 
            ITestInterface * This,
            int a,
            LPCSTR b);
        
        END_INTERFACE
    } ITestInterfaceVtbl;

    interface ITestInterface
    {
        CONST_VTBL struct ITestInterfaceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITestInterface_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITestInterface_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITestInterface_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITestInterface_TestMethodWithReturnNoArgs(This)	\
    ( (This)->lpVtbl -> TestMethodWithReturnNoArgs(This) ) 

#define ITestInterface_TestMethodWithoutReturnNoArgs(This)	\
    ( (This)->lpVtbl -> TestMethodWithoutReturnNoArgs(This) ) 

#define ITestInterface_TestMethodWithoutReturnWith2Args(This,a,b)	\
    ( (This)->lpVtbl -> TestMethodWithoutReturnWith2Args(This,a,b) ) 

#define ITestInterface_TestMethodWithReturnWith2Args(This,a,b)	\
    ( (This)->lpVtbl -> TestMethodWithReturnWith2Args(This,a,b) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITestInterface_INTERFACE_DEFINED__ */


#ifndef __ITestInterface2_INTERFACE_DEFINED__
#define __ITestInterface2_INTERFACE_DEFINED__

/* interface ITestInterface2 */
/* [object][local][version][uuid] */ 


EXTERN_C const IID IID_ITestInterface2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1828CCF9-0AE4-493D-A0A7-51C56C6226E9")
    ITestInterface2 : public IUnknown
    {
    public:
        virtual int STDMETHODCALLTYPE TestMethodInterface2( 
            int x) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITestInterface2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITestInterface2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITestInterface2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITestInterface2 * This);
        
        int ( STDMETHODCALLTYPE *TestMethodInterface2 )( 
            ITestInterface2 * This,
            int x);
        
        END_INTERFACE
    } ITestInterface2Vtbl;

    interface ITestInterface2
    {
        CONST_VTBL struct ITestInterface2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITestInterface2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITestInterface2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITestInterface2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITestInterface2_TestMethodInterface2(This,x)	\
    ( (This)->lpVtbl -> TestMethodInterface2(This,x) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITestInterface2_INTERFACE_DEFINED__ */


#ifndef __ITestInterfaceNotImpl_INTERFACE_DEFINED__
#define __ITestInterfaceNotImpl_INTERFACE_DEFINED__

/* interface ITestInterfaceNotImpl */
/* [object][local][version][uuid] */ 


EXTERN_C const IID IID_ITestInterfaceNotImpl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B595A7F7-0AFD-4AF5-A2B0-5C1A7200C519")
    ITestInterfaceNotImpl : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE TestMethodNotImplemented( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITestInterfaceNotImplVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITestInterfaceNotImpl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITestInterfaceNotImpl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITestInterfaceNotImpl * This);
        
        void ( STDMETHODCALLTYPE *TestMethodNotImplemented )( 
            ITestInterfaceNotImpl * This);
        
        END_INTERFACE
    } ITestInterfaceNotImplVtbl;

    interface ITestInterfaceNotImpl
    {
        CONST_VTBL struct ITestInterfaceNotImplVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITestInterfaceNotImpl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITestInterfaceNotImpl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITestInterfaceNotImpl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITestInterfaceNotImpl_TestMethodNotImplemented(This)	\
    ( (This)->lpVtbl -> TestMethodNotImplemented(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITestInterfaceNotImpl_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


