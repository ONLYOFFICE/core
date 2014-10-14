#ifndef _BUILD_TYPES_CROSSPLATFORM_H_
#define _BUILD_TYPES_CROSSPLATFORM_H_

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

typedef unsigned long       DWORD;
#ifndef _XCODE
typedef int                 BOOL;
#endif
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               REAL, FLOAT, *PFLOAT;

typedef char			CHAR;
typedef short			SHORT;
typedef unsigned short	USHORT;
typedef long			LONG;
typedef int				INT;
typedef unsigned int	UINT, *PUINT;
typedef wchar_t			WCHAR;

#ifdef _LINUX_QT
#include <inttypes.h>
typedef int64_t     T_LONG64;
typedef uint64_t    T_ULONG64;
typedef T_LONG64    __int64;

typedef T_ULONG64   ULONG64;
typedef T_LONG64    LONG64;
typedef T_ULONG64     UINT64;
#else

#if (!defined (_MAC) && (!defined(MIDL_PASS) || defined(__midl)) && (!defined(_M_IX86) || (defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64)))
typedef __int64				T_LONG64;
typedef unsigned __int64	T_ULONG64;
#else
#if defined(_MAC) && defined(_MAC_INT_64)
typedef __int64				T_LONG64;
typedef unsigned __int64	T_ULONG64;
#else
typedef double T_LONG64;
typedef double T_ULONG64;
#endif //_MAC and int64
#endif

#endif

#ifndef VOID
typedef void			VOID, *LPVOID;
#endif

#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F        /* smallest such that 1.0+FLT_EPSILON != 1.0 */
#endif

typedef unsigned long		ULONG, ARGB;

typedef long HRESULT;

#ifdef WIN32
#include "winerror.h"
#else
#ifndef S_OK
#define S_OK                                   ((HRESULT)0x00000000L)
#define S_FALSE                                ((HRESULT)0x00000001L)
#endif
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef RGB
typedef int RGB;
#define RGB(r,g,b) ((r)<<16|(g)<<8|(b))
#endif


#define RELEASEMEM(pobject)\
{\
	if (pobject!=NULL)\
	{\
		free(pobject);\
		pobject=NULL;\
	}\
}
#define RELEASEOBJECT(pobject)\
{\
	if (pobject!=NULL)\
	{\
		delete pobject;\
		pobject=NULL;\
	}\
}
#define RELEASEARRAYOBJECTS(pobject)\
{\
	if (pobject!=NULL)\
	{\
		delete []pobject;\
		pobject=NULL;\
	}\
}

#define ADDREFINTERFACE(pinterface)\
{\
    if (pinterface!=NULL)\
    {\
        pinterface->AddRef();\
    }\
}
#define RELEASEINTERFACE(pinterface)\
{\
    if (pinterface!=NULL)\
    {\
        pinterface->Release();\
        pinterface=NULL;\
    }\
}

// VARIANT
#ifndef VARIANT

/* 0 == FALSE, -1 == TRUE */
typedef short VARIANT_BOOL;

#define VARIANT_TRUE ((VARIANT_BOOL)-1)
#define VARIANT_FALSE ((VARIANT_BOOL)0)

enum VARENUM
    {	VT_EMPTY	= 0,
    VT_NULL	= 1,
    VT_I2	= 2,
    VT_I4	= 3,
    VT_R4	= 4,
    VT_R8	= 5,
    VT_CY	= 6,
    VT_DATE	= 7,
    VT_BSTR	= 8,
    VT_DISPATCH	= 9,
    VT_ERROR	= 10,
    VT_BOOL	= 11,
    VT_VARIANT	= 12,
    VT_UNKNOWN	= 13,
    VT_DECIMAL	= 14,
    VT_I1	= 16,
    VT_UI1	= 17,
    VT_UI2	= 18,
    VT_UI4	= 19,
    VT_I8	= 20,
    VT_UI8	= 21,
    VT_INT	= 22,
    VT_UINT	= 23,
    VT_VOID	= 24,
    VT_HRESULT	= 25,
    VT_PTR	= 26,
    VT_SAFEARRAY	= 27,
    VT_CARRAY	= 28,
    VT_USERDEFINED	= 29,
    VT_LPSTR	= 30,
    VT_LPWSTR	= 31,
    VT_RECORD	= 36,
    VT_INT_PTR	= 37,
    VT_UINT_PTR	= 38,
    VT_FILETIME	= 64,
    VT_BLOB	= 65,
    VT_STREAM	= 66,
    VT_STORAGE	= 67,
    VT_STREAMED_OBJECT	= 68,
    VT_STORED_OBJECT	= 69,
    VT_BLOB_OBJECT	= 70,
    VT_CF	= 71,
    VT_CLSID	= 72,
    VT_VERSIONED_STREAM	= 73,
    VT_BSTR_BLOB	= 0xfff,
    VT_VECTOR	= 0x1000,
    VT_ARRAY	= 0x2000,
    VT_BYREF	= 0x4000,
    VT_RESERVED	= 0x8000,
    VT_ILLEGAL	= 0xffff,
    VT_ILLEGALMASKED	= 0xfff,
    VT_TYPEMASK	= 0xfff
    } ;

typedef VARIANT_BOOL _VARIANT_BOOL;

typedef unsigned short VARTYPE;
/*
struct tagVARIANT
    {
    union
        {
        struct __tagVARIANT
            {
            VARTYPE vt;
            WORD wReserved1;
            WORD wReserved2;
            WORD wReserved3;
            union
                {
                LONGLONG llVal;
                LONG lVal;
                BYTE bVal;
                SHORT iVal;
                FLOAT fltVal;
                DOUBLE dblVal;
                VARIANT_BOOL boolVal;
                _VARIANT_BOOL bool;
                SCODE scode;
                CY cyVal;
                DATE date;
                BSTR bstrVal;
                IUnknown *punkVal;
                IDispatch *pdispVal;
                SAFEARRAY *parray;
                BYTE *pbVal;
                SHORT *piVal;
                LONG *plVal;
                LONGLONG *pllVal;
                FLOAT *pfltVal;
                DOUBLE *pdblVal;
                VARIANT_BOOL *pboolVal;
                _VARIANT_BOOL *pbool;
                SCODE *pscode;
                CY *pcyVal;
                DATE *pdate;
                BSTR *pbstrVal;
                IUnknown **ppunkVal;
                IDispatch **ppdispVal;
                SAFEARRAY **pparray;
                VARIANT *pvarVal;
                PVOID byref;
                CHAR cVal;
                USHORT uiVal;
                ULONG ulVal;
                ULONGLONG ullVal;
                INT intVal;
                UINT uintVal;
                DECIMAL *pdecVal;
                CHAR *pcVal;
                USHORT *puiVal;
                ULONG *pulVal;
                ULONGLONG *pullVal;
                INT *pintVal;
                UINT *puintVal;
                struct __tagBRECORD
                    {
                    PVOID pvRecord;
                    IRecordInfo *pRecInfo;
                    } 	__VARIANT_NAME_4;
                } 	__VARIANT_NAME_3;
            } 	__VARIANT_NAME_2;
        DECIMAL decVal;
        } 	__VARIANT_NAME_1;
    } ;
typedef VARIANT *LPVARIANT;

typedef VARIANT VARIANTARG;

typedef VARIANT *LPVARIANTARG;
*/

#endif // #ifndef VARIANT

#endif //_BUILD_TYPES_CROSSPLATFORM_H_
