

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Mon Jul 12 15:06:02 2010
 */
/* Compiler settings for .\AVSImageShellConverter.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __AVSImageShellConverter_h__
#define __AVSImageShellConverter_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IImageShellConverter_FWD_DEFINED__
#define __IImageShellConverter_FWD_DEFINED__
typedef interface IImageShellConverter IImageShellConverter;
#endif 	/* __IImageShellConverter_FWD_DEFINED__ */


#ifndef __ImageShellConverter_FWD_DEFINED__
#define __ImageShellConverter_FWD_DEFINED__

#ifdef __cplusplus
typedef class ImageShellConverter ImageShellConverter;
#else
typedef struct ImageShellConverter ImageShellConverter;
#endif /* __cplusplus */

#endif 	/* __ImageShellConverter_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IImageShellConverter_INTERFACE_DEFINED__
#define __IImageShellConverter_INTERFACE_DEFINED__

/* interface IImageShellConverter */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IImageShellConverter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("907848C4-E268-45e1-AD3E-1C1E5E78A93D")
    IImageShellConverter : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IImageShellConverterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IImageShellConverter * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IImageShellConverter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IImageShellConverter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IImageShellConverter * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IImageShellConverter * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IImageShellConverter * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IImageShellConverter * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IImageShellConverterVtbl;

    interface IImageShellConverter
    {
        CONST_VTBL struct IImageShellConverterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IImageShellConverter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IImageShellConverter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IImageShellConverter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IImageShellConverter_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IImageShellConverter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IImageShellConverter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IImageShellConverter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IImageShellConverter_INTERFACE_DEFINED__ */



#ifndef __AVSImageShellConverterLib_LIBRARY_DEFINED__
#define __AVSImageShellConverterLib_LIBRARY_DEFINED__

/* library AVSImageShellConverterLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_AVSImageShellConverterLib;

EXTERN_C const CLSID CLSID_ImageShellConverter;

#ifdef __cplusplus

class DECLSPEC_UUID("556A0934-8517-4814-9578-55BA6AFCEA94")
ImageShellConverter;
#endif
#endif /* __AVSImageShellConverterLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


