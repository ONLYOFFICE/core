#pragma once

#ifndef __cplusplus
#error "C++ compiler required"
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifdef CreateFile
#undef CreateFile
#endif

/* Forward Declarations */ 
typedef interface IONLYOFFICEDocBuilderValue IONLYOFFICEDocBuilderValue;
typedef interface IONLYOFFICEDocBuilderContextScope IONLYOFFICEDocBuilderContextScope;
typedef interface IONLYOFFICEDocBuilderContext IONLYOFFICEDocBuilderContext;
typedef interface IONLYOFFICEDocBuilder IONLYOFFICEDocBuilder;

typedef class CONLYOFFICEDocBuilderValue CONLYOFFICEDocBuilderValue;
typedef class CONLYOFFICEDocBuilderContextScope CONLYOFFICEDocBuilderContextScope;
typedef class CONLYOFFICEDocBuilderContext CONLYOFFICEDocBuilderContext;
typedef class CONLYOFFICEDocBuilder CONLYOFFICEDocBuilder;

MIDL_INTERFACE("2637FDFA-8473-4CB8-B90B-C23CB949D009")
IONLYOFFICEDocBuilderValue : public IDispatch
{
public:
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateInstance( 
        /* [optional][in] */ VARIANT value) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsEmpty( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsNull( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsUndefined( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsBool( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsInt( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsDouble( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsString( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsFunction( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsObject( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsArray( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsTypedArray( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetLength( 
        /* [retval][out] */ long *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE ToBool( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE ToInt( 
        /* [retval][out] */ long *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE ToDouble( 
        /* [retval][out] */ double *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE ToString( 
        /* [retval][out] */ BSTR *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetProperty( 
        /* [in] */ BSTR name,
        /* [retval][out] */ IONLYOFFICEDocBuilderValue **result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Get( 
        /* [in] */ long index,
        /* [retval][out] */ IONLYOFFICEDocBuilderValue **result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetProperty( 
        /* [in] */ BSTR name,
        /* [in] */ IONLYOFFICEDocBuilderValue *value) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Set( 
        /* [in] */ long index,
        /* [in] */ IONLYOFFICEDocBuilderValue *value) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Call( 
        /* [in] */ BSTR name,
        /* [optional][in] */ VARIANT val1,
        /* [optional][in] */ VARIANT val2,
        /* [optional][in] */ VARIANT val3,
        /* [optional][in] */ VARIANT val4,
        /* [optional][in] */ VARIANT val5,
        /* [optional][in] */ VARIANT val6,
        /* [retval][out] */ IONLYOFFICEDocBuilderValue **result) = 0;
        
};

MIDL_INTERFACE("656ae95c-ae91-4dc0-88bf-0b770fc2d552")
IONLYOFFICEDocBuilderContextScope : public IDispatch
{
public:
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
};
    
MIDL_INTERFACE("0416975a-65c3-4015-85e4-55d9dafec5fc")
IONLYOFFICEDocBuilderContext : public IDispatch
{
public:
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateUndefined( 
        /* [retval][out] */ IONLYOFFICEDocBuilderValue **result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateNull( 
        /* [retval][out] */ IONLYOFFICEDocBuilderValue **result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateObject( 
        /* [retval][out] */ IONLYOFFICEDocBuilderValue **result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateArray( 
        /* [in] */ long length,
        /* [retval][out] */ IONLYOFFICEDocBuilderValue **result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateTypedArray( 
        /* [in] */ VARIANT buffer,
        /* [in] */ long length,
        /* [retval][out] */ IONLYOFFICEDocBuilderValue **result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGlobal( 
        /* [retval][out] */ IONLYOFFICEDocBuilderValue **result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateScope( 
        /* [retval][out] */ IONLYOFFICEDocBuilderContextScope **result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsError( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
};

MIDL_INTERFACE("0C07B7E7-86A4-42E1-8E42-2FA961992E0F")
IONLYOFFICEDocBuilder : public IDispatch
{
public:
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateInstance( void) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE OpenFile( 
        /* [in] */ BSTR path,
        /* [in] */ BSTR params,
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateFile( 
        /* [in] */ BSTR type,
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetTmpFolder( 
        /* [in] */ BSTR folder) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE SaveFile( 
        /* [in] */ BSTR type,
        /* [in] */ BSTR path,
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE CloseFile( void) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE ExecuteCommand( 
        /* [in] */ BSTR command,
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Run( 
        /* [in] */ BSTR path,
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE RunText( 
        /* [in] */ BSTR commands,
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetProperty( 
        /* [in] */ BSTR key,
        /* [in] */ BSTR value) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Execute( 
        /* [in] */ BSTR command,
        /* [retval][out] */ IONLYOFFICEDocBuilderValue **result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE WriteData( 
        /* [in] */ BSTR path,
        /* [in] */ BSTR value,
        /* [in] */ VARIANT_BOOL append) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsSaveWithDoctrendererMode( 
        /* [retval][out] */ VARIANT_BOOL *result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetContext( 
        /* [retval][out] */ IONLYOFFICEDocBuilderContext **result) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Initialize( void) = 0;
        
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Dispose( void) = 0;
        
};

class DECLSPEC_UUID("85C41585-25D7-40F1-9CC6-FA17052650F4")
CONLYOFFICEDocBuilderValue;

class DECLSPEC_UUID("c54e2b15-ff5b-45a2-aa15-89a02605c30c")
CONLYOFFICEDocBuilderContextScope;

class DECLSPEC_UUID("299250bb-16c2-4ab4-8a49-a0c350d66bb5")
CONLYOFFICEDocBuilderContext;

class DECLSPEC_UUID("9BF69F3C-1506-41B9-B8EE-2839948C02E9")
CONLYOFFICEDocBuilder;
