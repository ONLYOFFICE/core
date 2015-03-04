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
#define BOOL int
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
    #ifndef max
        #define max(a,b)            (((a) > (b)) ? (a) : (b))
    #endif

    #ifndef min
        #define min(a,b)            (((a) < (b)) ? (a) : (b))
    #endif
#else
    #ifndef S_OK
        #define S_OK                                   ((HRESULT)0x00000000L)
        #define S_FALSE                                ((HRESULT)0x00000001L)
    #endif
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

#endif //_BUILD_TYPES_CROSSPLATFORM_H_
