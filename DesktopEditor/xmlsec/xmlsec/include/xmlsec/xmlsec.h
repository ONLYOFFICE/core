/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 * General functions and forward declarations.
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_H__
#define __XMLSEC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <libxml/tree.h>

#include <xmlsec/version.h>
#include <xmlsec/exports.h>
#include <xmlsec/strings.h>

/***********************************************************************
 *
 * Basic types to make ports to exotic platforms easier
 *
 ***********************************************************************/
/**
 * xmlSecPtr:
 *
 * Void pointer.
 */
typedef void*                                   xmlSecPtr;

/**
 * xmlSecSize:
 *
 * Size of something. Should be typedef instead of define
 * but it will break ABI (todo).
 */
#ifdef XMLSEC_NO_SIZE_T
#define xmlSecSize                              unsigned int
#else  /* XMLSEC_NO_SIZE_T */
#define xmlSecSize                              size_t
#endif /* XMLSEC_NO_SIZE_T */

/**
 * XMLSEC_SIZE_BAD_CAST:
 * @val:        the value to cast
 *
 * Bad cast to xmlSecSize
 */
#define XMLSEC_SIZE_BAD_CAST(val)               ((xmlSecSize)(val))

/**
 * xmlSecByte:
 *
 * One byte. Should be typedef instead of define
 * but it will break ABI (todo).
 */
#define xmlSecByte                              unsigned char

/***********************************************************************
 *
 * Forward declarations
 *
 ***********************************************************************/
typedef struct _xmlSecKeyData                   xmlSecKeyData, *xmlSecKeyDataPtr;
typedef struct _xmlSecKeyDataStore              xmlSecKeyDataStore, *xmlSecKeyDataStorePtr;
typedef struct _xmlSecKeyInfoCtx                xmlSecKeyInfoCtx, *xmlSecKeyInfoCtxPtr;
typedef struct _xmlSecKey                       xmlSecKey, *xmlSecKeyPtr;
typedef struct _xmlSecKeyStore                  xmlSecKeyStore, *xmlSecKeyStorePtr;
typedef struct _xmlSecKeysMngr                  xmlSecKeysMngr, *xmlSecKeysMngrPtr;
typedef struct _xmlSecTransform                 xmlSecTransform, *xmlSecTransformPtr;
typedef struct _xmlSecTransformCtx              xmlSecTransformCtx, *xmlSecTransformCtxPtr;

#ifndef XMLSEC_NO_XMLDSIG
typedef struct _xmlSecDSigCtx                   xmlSecDSigCtx, *xmlSecDSigCtxPtr;
#endif /* XMLSEC_NO_XMLDSIG */

#ifndef XMLSEC_NO_XMLENC
typedef struct _xmlSecEncCtx                    xmlSecEncCtx, *xmlSecEncCtxPtr;
#endif /* XMLSEC_NO_XMLENC */

XMLSEC_EXPORT int                               xmlSecInit              (void);
XMLSEC_EXPORT int                               xmlSecShutdown          (void);
XMLSEC_EXPORT const xmlChar *                   xmlSecGetDefaultCrypto  (void);

/**
 * XMLSEC_CRYPTO:
 *
 * Macro. Deprecated. Defined for backward compatibility only. Do not use
 * in your code and use xmlSecGetDefaultCrypto() function instead.
 *
 * Returns the default crypto engine.
 */
#define XMLSEC_CRYPTO                          (xmlSecGetDefaultCrypto())

/***********************************************************************
 *
 * Version checking
 *
 ***********************************************************************/
/**
 * xmlSecCheckVersionExact:
 *
 * Macro. Returns 1 if the loaded xmlsec library version exactly matches
 * the one used to compile the caller, 0 if it does not or a negative
 * value if an error occurs.
 */
#define xmlSecCheckVersionExact()       \
    xmlSecCheckVersionExt(XMLSEC_VERSION_MAJOR, XMLSEC_VERSION_MINOR, XMLSEC_VERSION_SUBMINOR, xmlSecCheckVersionExactMatch)

/**
 * xmlSecCheckVersion:
 *
 * Macro. Returns 1 if the loaded xmlsec library version ABI compatible with
 * the one used to compile the caller, 0 if it does not or a negative
 * value if an error occurs.
 */
#define xmlSecCheckVersion()    \
    xmlSecCheckVersionExt(XMLSEC_VERSION_MAJOR, XMLSEC_VERSION_MINOR, XMLSEC_VERSION_SUBMINOR, xmlSecCheckVersionABICompatible)

/**
 * xmlSecCheckVersionMode:
 * @xmlSecCheckVersionExactMatch:       the version should match exactly.
 * @xmlSecCheckVersionABICompatible:    the version should be ABI compatible.
 *
 * The xmlsec library version mode.
 */
typedef enum {
    xmlSecCheckVersionExactMatch = 0,
    xmlSecCheckVersionABICompatible
} xmlSecCheckVersionMode;

XMLSEC_EXPORT int       xmlSecCheckVersionExt   (int major,
                                                 int minor,
                                                 int subminor,
                                                 xmlSecCheckVersionMode mode);

/**
 * ATTRIBUTE_UNUSED:
 *
 * Macro used to signal to GCC unused function parameters
 */
#ifdef __GNUC__
#ifdef HAVE_ANSIDECL_H
#include <ansidecl.h>
#endif
#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED
#endif
#else
#define ATTRIBUTE_UNUSED
#endif

/***********************************************************************
 *
 * Helpers to convert from void* to function pointer, this silence
 * gcc warning
 *
 *     warning: ISO C forbids conversion of object pointer to function
 *     pointer type
 *
 * The workaround is to declare a union that does the conversion. This is
 * guaranteed (ISO/IEC 9899:1990 "C89"/"C90") to match exactly.
 *
 ***********************************************************************/

/**
 * XMLSEC_PTR_TO_FUNC_IMPL:
 * @func_type:          the function type.
 *
 * Macro declares helper functions to convert between "void *" pointer and
 * function pointer.
 */
#define XMLSEC_PTR_TO_FUNC_IMPL(func_type) \
    union xmlSecPtrToFuncUnion_ ##func_type { \
        void *ptr; \
        func_type * func; \
    } ; \
    static func_type * xmlSecPtrToFunc_ ##func_type(void * ptr) { \
         union xmlSecPtrToFuncUnion_ ##func_type x; \
         x.ptr = ptr; \
         return (x.func); \
    } \
    static void * xmlSecFuncToPtr_ ##func_type(func_type * func) { \
         union xmlSecPtrToFuncUnion_ ##func_type x; \
         x.func = func; \
         return (x.ptr); \
    }

/**
 * XMLSEC_PTR_TO_FUNC:
 * @func_type:          the function type.
 * @ptr:                the "void*" pointer to be converted.
 *
 * Macro converts from "void*" pointer to "func_type" function pointer.
 */
#define XMLSEC_PTR_TO_FUNC(func_type, ptr) \
    xmlSecPtrToFunc_ ##func_type((ptr))

/**
 * XMLSEC_FUNC_TO_PTR:
 * @func_type:          the function type.
 * @func:               the "func_type" function pointer to be converted.
 *
 * Macro converts from "func_type" function pointer to "void*" pointer.
 */
#define XMLSEC_FUNC_TO_PTR(func_type, func) \
    xmlSecFuncToPtr_ ##func_type((func))


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_H__ */


