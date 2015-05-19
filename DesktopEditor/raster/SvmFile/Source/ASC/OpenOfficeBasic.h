#pragma once
//#include "../tools/errcode.hxx"


namespace SVMCore{

#define M_PI 3.1415

//template<typename T> inline T Min(T a, T b) { return (a<b?a:b); }
//template<typename T> inline T Max(T a, T b) { return (a>b?a:b); }
//template<typename T> inline T Abs(T a) { return (a>=0?a:-a); }

#if defined(_WIN32) || defined(_WIN64)
	typedef __int64                  sal_Int64;
	typedef unsigned __int64         sal_uInt64;
#else
	typedef int64_t                  sal_Int64;
	typedef uint64_t         sal_uInt64;
#endif

#define SAL_TYPES_SIZEOFSHORT 2 
#define SAL_TYPES_SIZEOFLONG 4
#define SAL_W32
#define SAL_TYPES_SIZEOFPOINTER 4 


#define STRING_NOTFOUND    ((xub_StrLen)0xFFFF)
#define STRING_MATCH	   ((xub_StrLen)0xFFFF)
#define STRING_LEN		   ((xub_StrLen)0xFFFF)
#define STRING_MAXLEN	   ((xub_StrLen)0xFFFF)

// By changes you must also change: rsc/vclrsc.hxx
enum MapUnit { MAP_100TH_MM, MAP_10TH_MM, MAP_MM, MAP_CM,
			   MAP_1000TH_INCH, MAP_100TH_INCH, MAP_10TH_INCH, MAP_INCH,
			   MAP_POINT, MAP_TWIP, MAP_PIXEL, MAP_SYSFONT, MAP_APPFONT,
			   MAP_RELATIVE, /*MAP_REALAPPFONT,*/ MAP_LASTENUMDUMMY };


/********************************************************************************/
/* Useful defines
 */
  
/* The following SAL_MIN_INTn defines codify the assumption that the signed
 * sal_Int types use two's complement representation.  Defining them as
 * "-0x7F... - 1" instead of as "-0x80..." prevents warnings about applying the
 * unary minus operator to unsigned quantities.
 */
#define SAL_MIN_INT8          ((sal_Int8)   (-0x7F - 1))
#define SAL_MAX_INT8          ((sal_Int8)   0x7F)
#define SAL_MAX_UINT8         ((sal_uInt8)  0xFF)
#define SAL_MIN_INT16         ((sal_Int16)  (-0x7FFF - 1))
#define SAL_MAX_INT16         ((sal_Int16)  0x7FFF)
#define SAL_MAX_UINT16        ((sal_uInt16) 0xFFFF)
#define SAL_MIN_INT32         ((sal_Int32)  (-0x7FFFFFFF - 1))
#define SAL_MAX_INT32         ((sal_Int32)  0x7FFFFFFF)
#define SAL_MAX_UINT32        ((sal_uInt32) 0xFFFFFFFF)
#define SAL_MIN_INT64         ((sal_Int64)  (SAL_CONST_INT64(-0x7FFFFFFFFFFFFFFF) - 1))
#define SAL_MAX_INT64         ((sal_Int64)  SAL_CONST_INT64(0x7FFFFFFFFFFFFFFF))
#define SAL_MAX_UINT64        ((sal_uInt64) SAL_CONST_UINT64(0xFFFFFFFFFFFFFFFF))

#if SAL_TYPES_SIZEOFLONG == 4
#define SAL_MAX_SSIZE		SAL_MAX_INT32
#define SAL_MAX_SIZE		SAL_MAX_UINT32
#elif SAL_TYPES_SIZEOFLONG == 8
#define SAL_MAX_SSIZE		SAL_MAX_INT64
#define SAL_MAX_SIZE		SAL_MAX_UINT64
#endif


//\cosv\inc\cosv\csv_env.hxx
// Exact length builtin types
typedef signed char     INT8;
typedef unsigned char   UINT8;
typedef short           INT16;
typedef unsigned short  UINT16;
typedef long            INT32;
typedef unsigned long   UINT32;
typedef float		    REAL32;
typedef double		    REAL64;


// Additional builtin types
typedef INT32		 intt;		// standard sized integer.
typedef UINT32		 uintt;		// standard sized unsigned integer.
typedef REAL64 		 real;      // standard sized real.

//  Constants
//  ---------
// Zero-pointer for use in ellipsed (...) parameter lists which expect a
//   pointer which may have another size than an int.
// 	 Must be a define to be used in precompiled headers:
#define NIL   ((void*)0)
// char '\0'
#define NULCH '\0'



// Boolesche Operatoren
#define AND &&
#define OR  ||
#define NOT !

// Macro for distinguishing dynamic allocated pointers from
//   referencing pointers
#define DYN		// Exact specification: DYN has to be used if and only if:
				//	1. DYN specifies a class member pointer or reference variable and
				//     the class must free the referenced memory.
				//  2. DYN specifies a pointer or reference (return-) parameter of a function
				//     and for in-parameters the function or its class
				//     must free the referenced memory, the parameter is then called
				//     a let-parameter.
				//     For out- and inout-parameters
				//	   or return values the caller of the function hast to
				//     free the referenced memory.
				//
				//     It is irrelevant who allocated the memory!
				//
				//     DYN - variables use the prefixes "dp" or "dr" instead of "p" or "r".


//******        Assertions          ******//



//\sc\addin\inc\addin.h
#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif

#undef NULL
#define NULL            0

typedef unsigned char		BOOL;
typedef unsigned char		BYTE;
typedef unsigned short		USHORT;
typedef unsigned long		ULONG;

//\vos\inc\vos\macros.hxx
#ifndef VOS_BOUND
#	define VOS_BOUND(x,l,h)        ((x) <= (l) ? (l) : ((x) >= (h) ? (h) : (x)))
#endif


//\sal\inc\sal\types.h
//#if defined(SAL_W32) || defined(SAL_OS2) || defined(SAL_UNX)
#   define SAL_MAX_ENUM 0x7fffffff
//#elif defined(SAL_W16)
//#   define SAL_MAX_ENUM 0x7fff
//#endif
/* Boolean */
typedef unsigned char sal_Bool;
#   define sal_False ((sal_Bool)0)                
#   define sal_True  ((sal_Bool)1) 
  
/* char is assumed to always be 1 BYTE long */
typedef signed char         sal_Int8;
typedef unsigned char       sal_uInt8;

#if SAL_TYPES_SIZEOFSHORT == 2
	typedef signed short      sal_Int16;
	typedef unsigned short    sal_uInt16;
#else
     #error "Could not find 16-bit type, add support for your architecture"
#endif

#if SAL_TYPES_SIZEOFLONG == 4
	typedef signed long       sal_Int32;
	typedef unsigned long     sal_uInt32;
    #define SAL_PRIdINT32 "ld"
    #define SAL_PRIuUINT32 "lu"
    #define SAL_PRIxUINT32 "lx"
    #define SAL_PRIXUINT32 "lX"
#elif SAL_TYPES_SIZEOFINT == 4
	typedef signed int        sal_Int32;
	typedef unsigned int      sal_uInt32;
    #define SAL_PRIdINT32 "d"
    #define SAL_PRIuUINT32 "u"
    #define SAL_PRIxUINT32 "x"
    #define SAL_PRIXUINT32 "X"
#else
     #error "Could not find 32-bit type, add support for your architecture"
#endif

typedef char                     sal_Char;
typedef signed char              sal_sChar;
typedef unsigned char            sal_uChar;

#if ( defined(SAL_W32) && !defined(__MINGW32__) )
	typedef wchar_t             sal_Unicode;
#else
	#define SAL_UNICODE_NOTEQUAL_WCHAR_T
	typedef sal_uInt16          sal_Unicode;
#endif

typedef sal_Unicode xub_Unicode;
typedef sal_Unicode xub_uUnicode;
  
typedef void *                   sal_Handle;

/* sal_Size should currently be the native width of the platform */
#if SAL_TYPES_SIZEOFPOINTER == 4
	typedef sal_uInt32          sal_Size;
	typedef sal_Int32           sal_sSize;
#elif SAL_TYPES_SIZEOFPOINTER == 8
	typedef sal_uInt64          sal_Size;
	typedef sal_Int64           sal_sSize;
#else
	#error "Please make sure SAL_TYPES_SIZEOFPOINTER is defined for your architecture/compiler"
#endif
  
/* sal_PtrDiff holds the result of a pointer subtraction */
#if SAL_TYPES_SIZEOFPOINTER == 4
	typedef sal_Int32           sal_PtrDiff;
#elif SAL_TYPES_SIZEOFPOINTER == 8
	typedef sal_Int64           sal_PtrDiff;
#else
	#error "Please make sure SAL_TYPES_SIZEOFPOINTER is defined for your architecture/compiler"
#endif



//\tools\inc\tools\stream.hxx
// read, write, create,... options
#define STREAM_READ 					0x0001	// allow read accesses
#define STREAM_WRITE					0x0002	// allow write accesses

//\tools\inc\tools\solar.h
#ifndef F_PI
#define F_PI		3.14159265358979323846
#endif
#ifndef F_PI2
#define F_PI2		1.57079632679489661923
#endif
#ifndef F_PI4
#define F_PI4		0.785398163397448309616
#endif
#ifndef F_PI180
#define F_PI180 	0.01745329251994
#endif
#ifndef F_PI1800
#define F_PI1800	0.001745329251994
#endif
#ifndef F_PI18000
#define F_PI18000	0.0001745329251994
#endif
#ifndef F_2PI
#define F_2PI		6.28318530717958647694
#endif

#ifdef __cplusplus
template<typename T> inline T Min(T a, T b) { return (a<b?a:b); }
template<typename T> inline T Max(T a, T b) { return (a>b?a:b); }
template<typename T> inline T Abs(T a) { return (a>=0?a:-a); }
#endif

#ifdef STRING32
#define xub_StrLen		sal_uInt32
#else
#define xub_StrLen		USHORT
#endif


//AVS end

typedef sal_uInt16 rtl_TextEncoding;
typedef rtl_TextEncoding CharSet;


/* sal_IntPtr, sal_uIntPtr are integer types designed to hold pointers so that any valid
 * pointer to void can be converted to this type and back to a pointer to void and the 
 * result will compare to the original pointer */
#if SAL_TYPES_SIZEOFPOINTER == 4
	typedef sal_Int32           sal_IntPtr;
	typedef sal_uInt32          sal_uIntPtr;
    #define SAL_PRIdINTPTR SAL_PRIdINT32
    #define SAL_PRIuUINTPTR SAL_PRIuUINT32
    #define SAL_PRIxUINTPTR SAL_PRIxUINT32
    #define SAL_PRIXUINTPTR SAL_PRIXUINT32
#elif SAL_TYPES_SIZEOFPOINTER == 8
	typedef sal_Int64           sal_IntPtr;
	typedef sal_uInt64          sal_uIntPtr;
    #define SAL_PRIdINTPTR SAL_PRIdINT64
    #define SAL_PRIuUINTPTR SAL_PRIuUINT64
    #define SAL_PRIxUINTPTR SAL_PRIxUINT64
    #define SAL_PRIXUINTPTR SAL_PRIXUINT64
#else
	#error "Please make sure SAL_TYPES_SIZEOFPOINTER is defined for your architecture/compiler"
#endif

}//SVMCore